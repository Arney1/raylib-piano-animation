#include "../graphics/bresenham.h"
#include "../graphics/midcircle.h"
#include "../objects/note_block.h"
#include "../objects/piano.h"
#include "../render/render_state.h"
#include "../utils/color_palette.h"
#include "../utils/screen_types.h"
#include "anim_screen.h"
#include "raylib.h"
#include <stdlib.h>
// #include "../graphics/ellipse.h"

#define MAX_NOTES 128

static NoteBlock notes[MAX_NOTES];

static Piano piano;

void spawn_note_random(void) {
  for (int i = 0; i < MAX_NOTES; i++) {
    if (!notes[i].active) {

      bool is_black = rand() % 2;
      int key = rand() % (!is_black ? piano.white_count : piano.black_count);

      Rectangle r = is_black ? piano.black_keys[key] : piano.white_keys[key];

      note_init(&notes[i], r.x, 0, r.width, rand() % 100 + 50, key, is_black);

      break;
    }
  }
}

void anim_screen_init(void) {
  piano_init(&piano, (Vector2){0, SCREEN_H * 3 / 4},
             (Vector2){SCREEN_W - 1, SCREEN_H - 1});

  for (int i = 0; i < MAX_NOTES; i++) {
    notes[i].active = false;
  }
}

void anim_screen_update(void) {

  if (IsKeyPressed(KEY_O)) {
    gRenderMode =
        (gRenderMode == RENDER_FILLED) ? RENDER_OUTLINE : RENDER_FILLED;
  }
  if (GetRandomValue(0, 50) == 0) {
    spawn_note_random();
  }
  float piano_y = SCREEN_H * 3 / 4;

  for (int i = 0; i < MAX_NOTES; i++) {
    NoteBlock *n = &notes[i];

    if (!n->active)
      continue;

    note_update(n);

    float top = n->y + n->height;
    float bottom = n->y;

    if (!n->triggered && top >= piano_y) {
      if (n->is_black)
        piano_set_black(&piano, n->key_index, true);
      else
        piano_set_white(&piano, n->key_index, true);

      n->triggered = true;
    }

    if (n->triggered && bottom >= piano_y) {
      if (n->is_black)
        piano_set_black(&piano, n->key_index, false);
      else
        piano_set_white(&piano, n->key_index, false);

      n->active = false;
    }
  }
}

void anim_screen_draw(void) {

  ClearBackground(COLOR_BASE);
  for (int i = 0; i < MAX_NOTES; i++) {
    note_draw(&notes[i]);
  }
  piano_draw(&piano);
}

void anim_screen_unload(void) { piano_clear(&piano); }
