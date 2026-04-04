#ifndef APP_H
#define APP_H

#include "../utils/screen_types.h"

void App_Init(void);
void App_Update(void);
void App_Draw(void);
void App_Unload(void);
bool App_ShouldClose(void);

#endif
