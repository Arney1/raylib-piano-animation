#ifndef MENU_SCREEN_H
#define MENU_SCREEN_H

#include "../utils/screen_types.h"

void menu_screen_init(void);
void menu_screen_update(Screen *currentScreen);
void menu_screen_draw(void);
void menu_screen_unload(void);

#endif
