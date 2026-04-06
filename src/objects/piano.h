#ifndef PIANO_H
#define PIANO_H

#include "../utils/screen_types.h"
#include <raylib.h>
#include <stdbool.h>

#define MAX_WHITE_KEYS (7*7 + 1)
#define MAX_BLACK_KEYS (7*5)
#define MAX_KEYS (MAX_WHITE_KEYS + MAX_BLACK_KEYS)

typedef struct {
    Rectangle white_keys[MAX_WHITE_KEYS];
    Rectangle black_keys[MAX_BLACK_KEYS];
    bool white_active[MAX_WHITE_KEYS];
    bool black_active[MAX_BLACK_KEYS];
    int white_count;
    int black_count;
    bool show_musical_scale;
} Piano;

void piano_init(Piano *p, Vector2 start, Vector2 end);
void piano_draw(Piano *p);
void piano_set_white(Piano *p, int i, bool state);
void piano_set_black(Piano *p, int i, bool state);
void piano_clear(Piano *p);
void piano_toggle_musical_scale(Piano *p);

#endif
