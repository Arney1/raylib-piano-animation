#include "menu_screen.h"
#include "../graphics/bresenham.h"
#include "../utils/color_palette.h"
#include "../utils/screen_types.h"
#include "anim_screen.h"
#include <stdlib.h>

#include "raylib.h"
static int clickedProgram = 0;

void MenuScreen_Init(void) { clickedProgram = 0; }

void MenuScreen_Update(Screen *currentScreen) {
  if (IsKeyPressed(KEY_ENTER)) {
    *currentScreen = SCREEN_ANIM;
    anim_screen_init();
  }
  // if (IsKeyPressed(KEY_A)) {
  //     *currentScreen = SCREEN_ABOUT;
  // }
}

void MenuScreen_Draw(void) { ClearBackground(COLOR_BASE); }
