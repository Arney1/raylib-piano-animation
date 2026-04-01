#include "../utils/draw_utils.h"
#include "note_block.h"

void note_init(NoteBlock *n, float x, float y, float w, float h, int key_index,
               bool is_black) {
  n->x = x;
  n->y = y;
  n->width = w;
  n->height = h;
  n->key_index = key_index;
  n->is_black = is_black;
  n->active = true;
  n->triggered = false;
}

void note_update(NoteBlock *n) {
  if (!n->active)
    return;

  n->y += NOTE_SPEED;
}

void note_draw(NoteBlock *n) {
  if (!n->active)
    return;

  Color top =
      n->is_black ? (Color){120, 120, 255, 255} : (Color){255, 200, 100, 255};

  Color bottom =
      n->is_black ? (Color){60, 60, 200, 255} : (Color){255, 120, 60, 255};

  DrawRectGradientSmart(n->x, n->y, n->width, n->height, top, bottom);
}
