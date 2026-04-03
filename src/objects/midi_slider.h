#ifndef MIDI_SLIDER_H
#define MIDI_SLIDER_H

#include "raylib.h"
#include <stdbool.h>

bool midi_slider_update(Rectangle bounds, float *elapsed_time, float total_time);
void midi_slider_draw(Rectangle bounds, float elapsed_time, float total_time);

#endif
