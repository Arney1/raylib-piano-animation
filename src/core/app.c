#include "../screens/about_screen.h"
#include "../screens/anim_screen.h"
#include "../screens/menu_screen.h"
#include "../screens/objects_screen.h"
#include "../utils/screen_types.h"
#include "app.h"

static Screen currentScreen;

void App_Init(void) {
  currentScreen = SCREEN_MENU;
  menu_screen_init();
}

void App_Update(void) {
  switch (currentScreen) {
  case SCREEN_MENU:
    menu_screen_update(&currentScreen);
    break;
  case SCREEN_ANIM:
    anim_screen_update(&currentScreen);
    break;
  case SCREEN_OBJECTS:
    objects_screen_update(&currentScreen);
    break;
  case SCREEN_ABOUT:
    about_screen_update(&currentScreen);
    break;
  case SCREEN_EXIT:
    break;
  }
}

void App_Draw(void) {
  switch (currentScreen) {
  case SCREEN_MENU:
    menu_screen_draw();
    break;
  case SCREEN_ANIM:
    anim_screen_draw();
    break;
  case SCREEN_OBJECTS:
    objects_screen_draw();
    break;
  case SCREEN_ABOUT:
    about_screen_draw();
    break;
  case SCREEN_EXIT:
    break;
  }
}

void App_Unload(void) {}
bool App_ShouldClose(void) { return currentScreen == SCREEN_EXIT; }
