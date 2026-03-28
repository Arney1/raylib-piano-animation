#include "app.h"
#include "../screens/menu_screen.h"
#include "../screens/anim_screen.h"
// #include "../screens/about_screen.h"

static Screen currentScreen;

void App_Init(void) {
    currentScreen = SCREEN_MENU;
    MenuScreen_Init();
}

void App_Update(void) {
    switch (currentScreen) {
        case SCREEN_MENU:
            MenuScreen_Update(&currentScreen);
            break;
        case SCREEN_ANIM:
            AnimScreen_Update();
            break;
        // case SCREEN_ABOUT:
        //     AboutScreen_Update(&currentScreen);
        //     break;
    }
}

void App_Draw(void) {
    switch (currentScreen) {
        case SCREEN_MENU:
            MenuScreen_Draw();
            break;
        case SCREEN_ANIM:
            AnimScreen_Draw();
            break;
        // case SCREEN_ABOUT:
        //     AboutScreen_Draw();
        //     break;
    }
}
void App_Unload(void) {
    // cleanup (boleh kosong dulu)
}
