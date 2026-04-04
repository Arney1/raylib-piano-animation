#include "../utils/draw_utils.h"
#include "note_block.h"

void note_init(NoteBlock *n, float x, float y, float w, float h, int key_index,
               bool is_black) {
  // inset x by 1px and shrink width by 2px so adjacent notes have a visible gap
  n->x = x + 1.0f;
  n->y = y;
  n->width = w - 2.0f;
  n->height = h;
  n->key_index = key_index;
  n->is_black = is_black;
  n->active = true;
  n->triggered = false;
  // constant speed, match fall_speed in anim_screen.c
  n->vy = 225.0f;
  n->t = 0;
}

void note_update(NoteBlock *n) {
  if (!n->active)
    return;

  float dt = GetFrameTime();
  n->t += dt;

  // linear motion move downward (pixels per second * seconds = pixels)
  n->y += n->vy * dt;
}

void note_draw(NoteBlock *n) {
  if (!n->active)
    return;

  Color top =
      n->is_black ? (Color){120, 120, 255, 255} : (Color){255, 200, 100, 255};

  Color bottom =
      n->is_black ? (Color){60, 60, 200, 255} : (Color){255, 120, 60, 255};

  DrawSquircleGradientSmart((Rectangle){n->x, n->y, n->width, n->height}, 0.2f,
                            top, bottom);
}
