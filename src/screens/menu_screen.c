#include "menu_screen.h"
#include "raylib.h"

void MenuScreen_Init(void) {
clickedProgram = 0;
}

void MenuScreen_Update(Screen *currentScreen) {
    if (IsKeyPressed(KEY_ENTER)) {
        *currentScreen = SCREEN_ANIM;
    }
    // if (IsKeyPressed(KEY_A)) {
    //     *currentScreen = SCREEN_ABOUT;
    // }
}

void MenuScreen_Draw(void) {
    DrawRectangleRounded((Rectangle){0, 0, 160, 36}, 0.3f, 6,
                             (Color){80, 130, 220, 255});
}
