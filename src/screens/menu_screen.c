#include "../objects/piano.h"
#include "../utils/color_palette.h"
#include "../utils/draw_utils.h"
#include "about_screen.h"
#include "anim_screen.h"
#include "menu_screen.h"
#include "objects_screen.h"
#include "raylib.h"

static int selectedIndex = 0;
static const int numOptions = 4;
static const char *menuOptions[] = {"Play Animation", "Objects", "About",
                                    "Exit"};
static Piano menuPiano;
static bool is_menu_active = false;

void menu_screen_init(void) {
  is_menu_active = true;
  piano_init(&menuPiano, (Vector2){0, SCREEN_H * 3 / 4},
             (Vector2){SCREEN_W - 1, SCREEN_H - 1});
}

void menu_screen_update(Screen *currentScreen) {
  if (IsKeyPressed(KEY_DOWN)) {
    selectedIndex = (selectedIndex + 1) % numOptions;
  }
  if (IsKeyPressed(KEY_UP)) {
    selectedIndex = (selectedIndex - 1 + numOptions) % numOptions;
  }

  if (IsKeyPressed(KEY_ENTER)) {
    switch (selectedIndex) {
    case 0:
      menu_screen_unload();
      *currentScreen = SCREEN_ANIM;
      anim_screen_init();
      break;
    case 2:
      menu_screen_unload();
      *currentScreen = SCREEN_ABOUT;
      about_screen_init();
      break;
    case 1:
      menu_screen_unload();
      *currentScreen = SCREEN_OBJECTS;
      objects_screen_init();
      break;
    case 3:
      menu_screen_unload();
      *currentScreen = SCREEN_EXIT;
      break;
    }
  }
}

void menu_screen_draw(void) {
  if (!is_menu_active)
    return;
  ClearBackground(COLOR_BASE);

  piano_draw(&menuPiano);

  DrawText("MIDI VISUALIZER", 50, 50, 40, COLOR_TEXT);
  DrawText("Use Up/Down Arrows & Enter to Select", 50, 100, 20,
           COLOR_TEXT_MUTED);

  int startY = 160;
  for (int i = 0; i < numOptions; i++) {
    bool isSelected = (i == selectedIndex);

    Color bgColor, textColor;

    if (i == 3) {
      bgColor = isSelected ? RED : COLOR_SURFACE;
      textColor = isSelected ? WHITE : (Color){255, 100, 100, 255};
    } else {
      bgColor = isSelected ? COLOR_PRIMARY : COLOR_SURFACE;
      textColor = isSelected ? COLOR_BASE : COLOR_TEXT;
    }

    Rectangle itemRec = {50, startY + (i * 50), 300, 40};
    DrawSquircleSmart(itemRec, bgColor, 0.4f);
    DrawText(menuOptions[i], itemRec.x + 20, itemRec.y + 10, 20, textColor);
  }
}

void menu_screen_unload(void) {
  is_menu_active = false;
  piano_clear(&menuPiano);
}
