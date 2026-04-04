#include "../objects/piano.h"
#include "audio.h"
#include "raylib.h"
#include <math.h>

#define BUFFER_SIZE 4096
#define SAMPLE_RATE 44100
#define MAX_VOICES 32
#define MASTER_VOLUME 1.0f
#define VOICE_MIX_LEVEL 0.25f
// controls how fast the note decays, larger means faster drop in volume
#define DECAY_K 0.00008f

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

  int white_idx = 0, black_idx = 0;
  // midi note 24 = C1, lowest key
  int base_midi_note = 24;
  for (int i = 0; i < 85; i++) {
    // equal temperament formula: freq = 440 * 2^((midi - 69) / 12)
    float freq = 440.0f * powf(2.0f, ((base_midi_note + i) - 69) / 12.0f);
    int note_in_octave = i % 12;
    // semitones 1,3,6,8,10 are the black keys in a chromatic scale
    bool is_black =
        (note_in_octave == 1 || note_in_octave == 3 || note_in_octave == 6 ||
         note_in_octave == 8 || note_in_octave == 10);
    if (is_black && black_idx < MAX_BLACK_KEYS)
      black_frequencies[black_idx++] = freq;
    else if (!is_black && white_idx < MAX_WHITE_KEYS)
      white_frequencies[white_idx++] = freq;
  }
}

void audio_play_note(int key_index, bool is_black) {
  float freq = 0.0f;
  if (is_black && key_index < MAX_BLACK_KEYS)
    freq = black_frequencies[key_index];
  else if (!is_black && key_index < MAX_WHITE_KEYS)
    freq = white_frequencies[key_index];
  if (freq == 0.0f)
    return;

  for (int i = 0; i < MAX_VOICES; i++) {
    if (!voices[i].active) {
      voices[i].frequency = freq;
      voices[i].phase = 0.0f;
      voices[i].volume = 0.0f;
      voices[i].decay_time = 0.0f; // reset decay clock
      voices[i].active = true;
      voices[i].releasing = false;
      break;
    }
  }
}

void audio_stop_note(int key_index, bool is_black) {
  float freq = 0.0f;
  if (is_black && key_index < MAX_BLACK_KEYS)
    freq = black_frequencies[key_index];
  else if (!is_black && key_index < MAX_WHITE_KEYS)
    freq = white_frequencies[key_index];

  // match by frequency with a small tolerance to handle floating point drift
  for (int i = 0; i < MAX_VOICES; i++) {
    if (voices[i].active && fabsf(voices[i].frequency - freq) < 1.0f)
      voices[i].releasing = true;
  }
}

void audio_update() {
  while (IsAudioStreamProcessed(stream)) {
    for (int i = 0; i < BUFFER_SIZE; i++)
      buffer[i] = 0.0f;

    for (int v = 0; v < MAX_VOICES; v++) {
      if (!voices[v].active)
        continue;

      // phase increment per sample, that is, how much of the sine wave to step
      // each tick
      // derived from: cycles_per_second / samples_per_second = fraction of full
      // cycle
      float phaseInc = (2.0f * PI * voices[v].frequency) / (float)SAMPLE_RATE;

      for (int i = 0; i < BUFFER_SIZE; i++) {
        if (voices[v].releasing) {
          // linear fade out on release, step of 0.011 per sample
          voices[v].volume -= 0.011f;
          if (voices[v].volume <= 0.0f) {
            voices[v].volume = 0.0f;
            voices[v].active = false;
            break;
          }
        } else {
          // ramp volume up by 0.01 per sample to avoid click or popping. its
          // called attack in audio world
          if (voices[v].volume < 1.0f)
            voices[v].volume += 0.01f;

          // logarithmic decay: 1 / (1 + k * t), approaches 0 but never reaches
          // it
          // decay_time counts samples elapsed since note start
          voices[v].decay_time += 1.0f;
          float decay = 1.0f / (1.0f + DECAY_K * voices[v].decay_time);
          // final sample = sine wave * envelope * decay * mix level * master
          // vol
          buffer[i] += sinf(voices[v].phase) * voices[v].volume * decay *
                       VOICE_MIX_LEVEL * MASTER_VOLUME;

          voices[v].phase += phaseInc;
          if (voices[v].phase >= 2.0f * PI)
            voices[v].phase -= 2.0f * PI;

          continue; // skip the non-decaying write below
        }

        // releasing path: no decay applied, just envelope * mix
        buffer[i] += sinf(voices[v].phase) * voices[v].volume *
                     VOICE_MIX_LEVEL * MASTER_VOLUME;

        voices[v].phase += phaseInc;
        if (voices[v].phase >= 2.0f * PI)
          voices[v].phase -= 2.0f * PI;
      }
    }

    UpdateAudioStream(stream, buffer, BUFFER_SIZE);
  }
}

void audio_unload() {
  UnloadAudioStream(stream);
  CloseAudioDevice();
}
