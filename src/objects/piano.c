#include "../graphics/bresenham.h"
#include "../render/render_state.h"
#include "../utils/draw_utils.h"
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
    // step xc forward by one white key width each iteration
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
  if (gRenderMode == RENDER_FILLED) {
    for (int i = 0; i < p->white_count; i++) {
      Rectangle r = p->white_keys[i];

      if (p->white_active[i]) {
        DrawRectangleGradientV(r.x, r.y, r.width, r.height,
                               (Color){255, 245, 230, 255},
                               (Color){255, 150, 80, 255});
      } else {
        DrawRectangle(r.x, r.y, r.width, r.height, WHITE);
      }

      DrawRectangleLines(r.x, r.y, r.width, r.height, BLACK);
    }

    for (int i = 0; i < p->black_count; i++) {
      Rectangle r = p->black_keys[i];

      if (p->black_active[i]) {
        DrawRectGradientSmart(r.x, r.y, r.width, r.height,
                              (Color){20, 20, 30, 255},
                              (Color){80, 80, 220, 255});
      } else {
        DrawRectangle(r.x, r.y, r.width, r.height, BLACK);
      }
    }
  } else {
    // base white piano structure
    int x_start = p->white_keys[0].x;
    int y_start = p->white_keys[0].y;

    Rectangle last = p->white_keys[p->white_count - 1];
    int x_end = last.x + last.width;
    int y_end = last.y + last.height;

    BresenhamLine(x_start, y_start, x_end, y_start, WHITE);
    BresenhamLine(x_start, y_end, x_end, y_end, WHITE);
    BresenhamLine(x_start, y_start, x_start, y_end, WHITE);
    BresenhamLine(x_end, y_start, x_end, y_end, WHITE);

    // cuts between white keys
    for (int i = 1; i < p->white_count; i++) {
      int x = p->white_keys[i].x;
      // divider starts at the top unless a black key is sitting on this
      // boundary
      int y_cut = y_start;

      for (int j = 0; j < p->black_count; j++) {
        Rectangle bk = p->black_keys[j];
        // check if this white key boundary falls inside a black key's x range
        if (x > bk.x && x < (bk.x + bk.width)) {
          // start the divider below the black key so it doesn't overlap
          y_cut = bk.y + bk.height;
          break;
        }
      }

      BresenhamLine(x, y_cut, x, y_end, WHITE);
    }

    for (int i = 0; i < p->white_count; i++) {
      if (p->white_active[i]) {
        Rectangle r = p->white_keys[i];

        BresenhamLine(r.x, r.y, r.x + r.width, r.y, YELLOW);
        BresenhamLine(r.x, r.y, r.x, r.y + r.height, YELLOW);
        BresenhamLine(r.x + r.width, r.y, r.x + r.width, r.y + r.height,
                      YELLOW);
        BresenhamLine(r.x, r.y + r.height, r.x + r.width, r.y + r.height,
                      YELLOW);
      }
    }

    for (int i = 0; i < p->black_count; i++) {
      Rectangle r = p->black_keys[i];
      Color c = p->black_active[i] ? SKYBLUE : WHITE;

      BresenhamLine(r.x, r.y, r.x + r.width, r.y, c);  // top
      BresenhamLine(r.x, r.y, r.x, r.y + r.height, c); // left
      BresenhamLine(r.x + r.width, r.y, r.x + r.width, r.y + r.height,
                    c); // right
      BresenhamLine(r.x, r.y + r.height, r.x + r.width, r.y + r.height,
                    c); // bottom
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
