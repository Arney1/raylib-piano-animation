#ifndef NOTE_BLOCK_H
#define NOTE_BLOCK_H

#include "raylib.h"
#include <stdbool.h>

#define NOTE_SPEED 2.0f

typedef struct {
    float x;
    float y;
    float width;
    float height;

    int key_index;     // which piano key it belongs to
    bool is_black;     // white or black key

    bool active;
    bool triggered;

    float vy;   // vertical velocity
    float t;
} NoteBlock;

void note_init(NoteBlock *n, float x, float y, float w, float h, int key_index, bool is_black);
void note_update(NoteBlock *n);
void note_draw(NoteBlock *n);

#endif
