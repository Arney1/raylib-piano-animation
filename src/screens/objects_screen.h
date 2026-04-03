#ifndef OBJECTS_SCREEN_H
#define OBJECTS_SCREEN_H

#include "../utils/screen_types.h"

void objects_screen_init(void);
void objects_screen_update(Screen *currentScreen);
void objects_screen_draw(void);
void objects_screen_unload(void);

#endif // OBJECTS_SCREEN_H
