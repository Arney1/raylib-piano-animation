#include "../graphics/bresenham.h"
#include "../render/render_state.h"
#include "../utils/draw_utils.h"
#include "piano.h"
#include <stdio.h>

void piano_init(Piano *p, Vector2 start, Vector2 end) {
  p->show_musical_scale = false;
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
  char *white_notes[] = {"C", "D", "E", "F", "G", "A", "B"};
  char *black_notes[] = {"C#", "D#", "F#", "G#", "A#"};
  Color colors_white[] = {LIGHTGRAY, PINK,   YELLOW, GREEN,
                          SKYBLUE,   PURPLE, BEIGE};
  Color colors_black[] = {GRAY, RED, GOLD, LIME, BLUE};

  if (gRenderMode == RENDER_FILLED) {
    for (int i = 0; i < p->white_count; i++) {
      Rectangle r = p->white_keys[i];
      Color color1 = (p->show_musical_scale) ? colors_white[i % 7] : WHITE;
      Color color2 = (p->show_musical_scale) ? colors_white[i % 7]
                                             : (Color){255, 150, 80, 255};
      color1.r = (color1.r + 510) / 3;
      color1.g = (color1.g + 510) / 3;
      color1.b = (color1.b + 510) / 3;
      color1.a = 255;

      if (p->white_active[i]) {
        DrawRectangleGradientV(r.x, r.y, r.width, r.height, color1, color2);
      } else {
        DrawRectangle(r.x, r.y, r.width, r.height, color1);
      }

      DrawRectangleLines(r.x, r.y, r.width, r.height, BLACK);
    }

    for (int i = 0; i < p->black_count; i++) {
      Rectangle r = p->black_keys[i];
      Color color1 = (p->show_musical_scale) ? colors_black[i % 7] : BLACK;
      Color color2 = (p->show_musical_scale) ? colors_black[i % 7]
                                             : (Color){255, 150, 80, 255};
      color1.r = (color1.r + 40) / 3;
      color1.g = (color1.g + 40) / 3;
      color1.b = (color1.b + 60) / 3;
      color1.a = 255;

      if (p->black_active[i]) {
        DrawRectGradientSmart(r.x, r.y, r.width, r.height, color1, color2);
      } else {
        DrawRectangle(r.x, r.y, r.width, r.height, color1);
      }
    }
  } else {
    // base white piano structure
    int x_start = p->white_keys[0].x;
    int y_start = p->white_keys[0].y;

    Rectangle last = p->white_keys[p->white_count - 1];
    int x_end = last.x + last.width;
    int y_end = last.y + last.height;

    // cuts between white keys
    // draw white key outlines
    for (int i = 0; i < p->white_count; i++) {
      Rectangle r = p->white_keys[i];

      Color color1;
      if (p->show_musical_scale) {
        color1.r = (WHITE.r + colors_white[i % 7].r) / 2;
        color1.g = (WHITE.g + colors_white[i % 7].g) / 2;
        color1.b = (WHITE.b + colors_white[i % 7].b) / 2;
        color1.a = 255;
      } else {
        color1 = WHITE;
      }

      // find y_cut for this key's left boundary
      int x = r.x;
      int y_cut = y_start;
      for (int j = 0; j < p->black_count; j++) {
        Rectangle bk = p->black_keys[j];
        if (x > bk.x && x < (bk.x + bk.width)) {
          y_cut = bk.y + bk.height;
          break;
        }
      }

      // find x_cut for right boundary too
      int x_right = r.x + r.width;
      int y_cut_right = y_start;
      for (int j = 0; j < p->black_count; j++) {
        Rectangle bk = p->black_keys[j];
        if (x_right > bk.x && x_right < (bk.x + bk.width)) {
          y_cut_right = bk.y + bk.height;
          break;
        }
      }

      Color draw_color = p->white_active[i] ? (p->show_musical_scale
                                                   ? colors_white[i % 7]
                                                   : (Color){255, 150, 80, 255})
                                            : color1;
      draw_color.a = 255;

      BresenhamLine(r.x, y_start, r.x + r.width, y_start,
                    WHITE);                                   // top (shared)
      BresenhamLine(r.x, y_end, r.x + r.width, y_end, WHITE); // bottom
      BresenhamLine(r.x, y_cut, r.x, y_end, draw_color);      // left divider
      BresenhamLine(r.x + r.width, y_cut_right, r.x + r.width, y_end,
                    draw_color); // right divider
    }

    for (int i = 0; i < p->black_count; i++) {
      Rectangle r = p->black_keys[i];
      Color c;
      if (p->black_active[i])
        c = (p->show_musical_scale) ? colors_black[i % 7]
                                    : (Color){255, 150, 80, 255};
      else {
        c = WHITE;
        if (p->show_musical_scale) {
          c.r = (c.r + colors_black[i % 7].r) / 2;
          c.g = (c.g + colors_black[i % 7].g) / 2;
          c.b = (c.b + colors_black[i % 7].b) / 2;
          c.a = (c.a + colors_black[i % 7].a) / 2;
        }
      }

      BresenhamLine(r.x, r.y, r.x + r.width, r.y, c);  // top
      BresenhamLine(r.x, r.y, r.x, r.y + r.height, c); // left
      BresenhamLine(r.x + r.width, r.y, r.x + r.width, r.y + r.height,
                    c); // right
      BresenhamLine(r.x, r.y + r.height, r.x + r.width, r.y + r.height,
                    c); // bottom
    }
  }
  if (p->show_musical_scale) {
    for (int i = 0; i < p->white_count; i++) {
      Rectangle r = p->white_keys[i];
      float text_y_white = (r.height) * 3 / 4 + r.y;
      float text_x_white = r.x + r.width / 2 - 9;

      DrawText(white_notes[i % 7], text_x_white, text_y_white, 30,
               (gRenderMode == RENDER_FILLED) ? BLACK : WHITE);
    }
    for (int i = 0; i < p->black_count; i++) {
      Rectangle r = p->black_keys[i];
      float text_y_white = (r.height) * 3 / 4 + r.y;
      float text_x_white = r.x + r.width / 2 - 7;

      DrawText(black_notes[i % 5], text_x_white, text_y_white, 10, WHITE);
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

void piano_toggle_musical_scale(Piano *p) {
  p->show_musical_scale = !p->show_musical_scale;
}

void piano_clear(Piano *p) {
  for (int i = 0; i < p->white_count; i++)
    p->white_active[i] = false;

  for (int i = 0; i < p->black_count; i++)
    p->black_active[i] = false;
}
