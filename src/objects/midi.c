#include "../audio/audio.h"
#include "midi.h"
#include <stdio.h>

#define MAX_MIDI_NOTES 5000
// how many seconds ahead to spawn notes before theyre supposed to hit
#define LOOKAHEAD_TIME 5.0f

static MidiNote notes[MAX_MIDI_NOTES];
static int current_index = 0;
static int note_count = 0;

// returns the total length of the song based on the last note
float midi_get_length(void) {
  if (note_count == 0)
    return 0.0f;
  // last note's end time = total song duration
  return notes[note_count - 1].end;
}

// fast forward or goes back
void midi_seek(float target_time) {
  current_index = 0;
  // fast forward until it find the first note that plays after the target time
  while (current_index < note_count &&
         notes[current_index].start < target_time) {
    current_index++;
  }
}

void midi_load(const char *filename) {
  FILE *f = fopen(filename, "r");
  if (!f) {
    printf("Failed to open MIDI CSV\n");
    return;
  }

  note_count = 0;

  // skip header
  char line[256];
  fgets(line, sizeof(line), f);

  while (fgets(line, sizeof(line), f)) {
    MidiNote n;
    int program;
    char is_drum_str[16]; // buffer to hold true or false

    // %[^,] : read the string until the next comma
    // program, is_drum, start, end, pitch, velocity
    if (sscanf(line, "%d,%15[^,],%f,%f,%d,%d", &program, is_drum_str, &n.start,
               &n.end, &n.pitch, &n.velocity) == 6) {

      notes[note_count++] = n;

      if (note_count >= MAX_MIDI_NOTES)
        break;
    }
  }

  fclose(f);
  printf("Loaded %d MIDI notes\n", note_count);
}

void midi_reset() { current_index = 0; }

extern void spawn_note_from_pitch(int pitch, float duration,
                                  float time_until_hit);

void midi_update(float time) {
  // spawn notes when theyre within the lookahead window
  while (current_index < note_count &&
         notes[current_index].start <= time + LOOKAHEAD_TIME) {
    MidiNote *n = &notes[current_index];

    float duration = n->end - n->start;
    float time_until_hit = n->start - time; // calculate how long til it hits

    // shift pitch down one octave which is 12 semitones to fit the on-screen
    // keyboard range
    spawn_note_from_pitch(n->pitch - 12, duration, time_until_hit);

    current_index++;
  }
}
