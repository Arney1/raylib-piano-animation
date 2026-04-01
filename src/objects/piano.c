#include "../graphics/bresenham.h"
#include "piano.h"

void piano_init(Piano *p, Vector2 start, Vector2 end) {
  int x_start = start.x, x_end = end.x;
  int y_start = start.y, y_end = end.y;
  int xc = x_start;

  int white_w = (x_end - x_start) / (MAX_WHITE_KEYS - 1);
  int white_h = (y_end - y_start);

  p->white_count = MAX_WHITE_KEYS;
  p->black_count = 0;

  for (int i = 0; i < MAX_WHITE_KEYS - 1; i++) {
    p->white_keys[i] = (Rectangle){xc, y_start, white_w, white_h};
    xc += white_w;
    p->white_active[i] = false;
  }
  p->white_keys[MAX_WHITE_KEYS - 1] =
      (Rectangle){xc, y_start, white_w, white_h};
  p->white_active[MAX_WHITE_KEYS - 1] = false;

  int pattern[7] = {1, 1, 0, 1, 1, 1, 0};
  int black_w = white_w * 0.6;
  int black_h = (y_end - y_start) * 0.6;

  for (int i = 0; i < MAX_WHITE_KEYS - 1; i++) {
    if (pattern[i % 7]) {
      int x = x_start + i * white_w + white_w - black_w / 2;
      p->black_keys[p->black_count] = (Rectangle){x, y_start, black_w, black_h};
      p->black_active[p->black_count] = false;
      p->black_count++;
    }
  }
}

void piano_draw(Piano *p) {

  for (int i = 0; i < p->white_count; i++) {
    Rectangle r = p->white_keys[i];

    if (p->white_active[i]) {
      DrawRectangleGradientV(r.x, r.y, r.width, r.height,
                             (Color){255, 255, 255, 255},
                             (Color){255, 223, 184, 255});
    } else {
      DrawRectangleRec(r, WHITE);
    }

    DrawRectangleLines(r.x, r.y, r.width, r.height, BLACK);
  }

  for (int i = 0; i < p->black_count; i++) {
    Rectangle r = p->black_keys[i];

    if (p->black_active[i]) {
      DrawRectangleGradientV(r.x, r.y, r.width, r.height, (Color){0, 0, 0, 255},
                             (Color){128, 128, 128, 255});
    } else {
      DrawRectangleRec(r, BLACK);
    }
  }
}

void piano_set_white(Piano *p, int i, bool state) {
  if (i >= 0 && i < p->white_count)
    p->white_active[i] = state;
}

void piano_set_black(Piano *p, int i, bool state) {
  if (i >= 0 && i < p->black_count)
    p->black_active[i] = state;
}

void piano_clear(Piano *p) {
  for (int i = 0; i < p->white_count; i++)
    p->white_active[i] = false;

  for (int i = 0; i < p->black_count; i++)
    p->black_active[i] = false;
}
