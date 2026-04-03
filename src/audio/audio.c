#include "../objects/piano.h"
#include "audio.h"
#include "raylib.h"
#include <math.h>

#define BUFFER_SIZE 4096
#define SAMPLE_RATE 44100
#define MAX_VOICES 32
#define MASTER_VOLUME 1.0f    // global output
#define VOICE_MIX_LEVEL 0.25f // a single voice

static Voice voices[MAX_VOICES] = {0};
static AudioStream stream;
static float buffer[BUFFER_SIZE] = {0};

static float white_frequencies[MAX_WHITE_KEYS];
static float black_frequencies[MAX_BLACK_KEYS];

void audio_init() {
  InitAudioDevice();
  SetAudioStreamBufferSizeDefault(BUFFER_SIZE);
  stream = LoadAudioStream(SAMPLE_RATE, 32, 1);
  SetAudioStreamPan(stream, 0.5f);
  PlayAudioStream(stream);

  // calculate frequencies for white and black keys, and store them
  int white_idx = 0;
  int black_idx = 0;
  int base_midi_note = 24; // C1

  for (int i = 0; i < 85; i++) {
    float freq = 440.0f * powf(2.0f, ((base_midi_note + i) - 69) / 12.0f);

    // check if key in the octave is black or white
    int note_in_octave = i % 12;
    bool is_black =
        (note_in_octave == 1 || note_in_octave == 3 || note_in_octave == 6 ||
         note_in_octave == 8 || note_in_octave == 10);

    if (is_black && black_idx < MAX_BLACK_KEYS) {
      black_frequencies[black_idx] = freq;
      black_idx++;
    } else if (!is_black && white_idx < MAX_WHITE_KEYS) {
      white_frequencies[white_idx] = freq;
      white_idx++;
    }
  }
}

void audio_play_note(int key_index, bool is_black) {
  float freq = 0.0f;

  // grab the correct freq from pre-calculated freqs arrays
  if (is_black && key_index < MAX_BLACK_KEYS) {
    freq = black_frequencies[key_index];
  } else if (!is_black && key_index < MAX_WHITE_KEYS) {
    freq = white_frequencies[key_index];
  }

  if (freq == 0.0f)
    return; // safety check

  for (int i = 0; i < MAX_VOICES; i++) {
    if (!voices[i].active) {
      voices[i].frequency = freq;
      voices[i].phase = 0.0f;
      voices[i].volume = 0.0f;
      voices[i].active = true;
      voices[i].releasing = false;
      break;
    }
  }
}

void audio_stop_note(int key_index, bool is_black) {
  float freq = 0.0f;

  if (is_black && key_index < MAX_BLACK_KEYS) {
    freq = black_frequencies[key_index];
  } else if (!is_black && key_index < MAX_WHITE_KEYS) {
    freq = white_frequencies[key_index];
  }

  for (int i = 0; i < MAX_VOICES; i++) {
    if (voices[i].active && fabs(voices[i].frequency - freq) < 1.0f) {
      voices[i].releasing = true;
    }
  }
}

void audio_update() {
  while (IsAudioStreamProcessed(stream)) {

    // wipe the buffer clean for this batch
    for (int i = 0; i < BUFFER_SIZE; i++) {
      buffer[i] = 0.0f;
    }

    // mix all active voices
    for (int v = 0; v < MAX_VOICES; v++) {
      if (!voices[v].active)
        continue;

      float phaseInc = (2.0f * PI * voices[v].frequency) / (float)SAMPLE_RATE;

      for (int i = 0; i < BUFFER_SIZE; i++) {

        // volume envelope processing
        if (voices[v].releasing) {
          voices[v].volume -= 0.011f; // fade out to mimic a piano key lifting
          if (voices[v].volume <= 0.0f) {
            voices[v].volume = 0.0f;
            voices[v].active = false;
            break; // voice is dead, stop processing its samples
          }
        } else {
          if (voices[v].volume < 1.0f) {
            voices[v].volume +=
                0.01f; // smooth fade in (prevents instant attack clicking)
          }
        }

        buffer[i] += sinf(voices[v].phase) * voices[v].volume *
                     VOICE_MIX_LEVEL * MASTER_VOLUME;

        // advance phase
        voices[v].phase += phaseInc;
        if (voices[v].phase >= 2.0f * PI) {
          voices[v].phase -= 2.0f * PI;
        }
      }
    }

    UpdateAudioStream(stream, buffer, BUFFER_SIZE);
  }
}

void audio_unload() {
  UnloadAudioStream(stream);
  CloseAudioDevice();
}
