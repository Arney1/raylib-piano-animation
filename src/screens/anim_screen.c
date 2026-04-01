#include "anim_screen.h"
#include "../graphics/bresenham.h"
#include "../graphics/midcircle.h"
#include "../objects/piano.h"
#include "../utils/color_palette.h"
#include "../utils/screen_types.h"
#include "raylib.h"
#include <stdlib.h>
// #include "../graphics/ellipse.h"

static float noteY = 0;
static Piano piano;

void anim_screen_init(void) {
  noteY = 0;
  piano_init(&piano, (Vector2){0, SCREEN_H * 3 / 4},
             (Vector2){SCREEN_W - 1, SCREEN_H - 1});
}

void anim_screen_update(void) {
  noteY += 2.0f;
  piano_activate_white(&piano, rand() % MAX_WHITE_KEYS);
  piano_activate_black(&piano, rand() % MAX_BLACK_KEYS);
}

void anim_screen_draw(void) {

  ClearBackground(COLOR_BASE);
  piano_draw(&piano);
  BresenhamRectangle(100, (int)noteY, 200, (int)noteY + 50,
                     (Color){80, 130, 220, 255});
}

void anim_screen_unload(void) {}
