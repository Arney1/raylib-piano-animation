#ifndef AUDIO_H
#define AUDIO_H

#include "raylib.h"
#include <stdbool.h>

typedef struct {
  float frequency;
  float phase;
  float volume;
  float decay_time;
  bool active;
  bool releasing;
} Voice;


void audio_init();
void audio_update();
void audio_play_note(int key_index, bool is_black);
void audio_stop_note(int key_index, bool is_black);
void audio_unload();

#endif
