#ifndef PIANO_H
#define PIANO_H

#include "../utils/screen_types.h"
#include <raylib.h>

void Piano_Init(void);
void Piano_Update(int *flip);
void Piano_Draw(Vector2 position_start, Vector2 position_end);
void Piano_Unload(void);

#endif
