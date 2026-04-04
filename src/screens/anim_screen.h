#ifndef ANIM_SCREEN_H
#define ANIM_SCREEN_H

#include "../utils/screen_types.h"

void get_key_from_pitch(int pitch, int *key_index, bool *is_black);
void spawn_note_from_pitch(int pitch, float duration, float time_until_hit);

void anim_screen_init(void);
void anim_screen_update(Screen *currentScreen);
void anim_screen_draw(void);
void anim_screen_unload(void);

#endif
