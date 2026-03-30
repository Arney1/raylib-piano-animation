#include "menu_screen.h"
#include "anim_screen.h"
#include "../graphics/bresenham.h"
#include "../utils/screen_types.h"
#include "../utils/color_palette.h"
#include <stdlib.h>

#include "raylib.h"
static int clickedProgram = 0;

void MenuScreen_Init(void) {
    clickedProgram = 0;
}

void MenuScreen_Update(Screen *currentScreen) {
    if (IsKeyPressed(KEY_ENTER)) {
        *currentScreen = SCREEN_ANIM;
        AnimScreen_Init();   // ✅ add this
    }
    // if (IsKeyPressed(KEY_A)) {
    //     *currentScreen = SCREEN_ABOUT;
    // }
}

void MenuScreen_Draw(void) {

    ClearBackground(COLOR_BASE);



    int x_start = 0, x_end = SCREEN_W;
    int xc = x_start;
    int y_start = SCREEN_H * 3 / 4;
    int y_end = SCREEN_H;
    int yc = y_start;

    int octaves = 7;
    int white_keys = 7*octaves;
    int white_key_width = SCREEN_W / white_keys;

    BresenhamLine(x_start, yc, x_end, yc, BLACK);
    yc += 1;
    BresenhamRectangle(x_start, yc, x_end, y_end, WHITE);
    for (int i = 0; i < white_keys; i++) {

        xc += white_key_width;
        BresenhamLine(xc, y_start, xc, y_end, BLACK);
    }

    int pattern[7] = {1, 1, 0, 1, 1, 1, 0};
    int black_key_width = white_key_width * 0.6;
    int black_key_height = (y_end - y_start) * 0.6;

    for (int i = 0; i < white_keys; i++) {
        if (pattern[i % 7]) {
            int x = i * white_key_width + white_key_width - black_key_width / 2;
            BresenhamRectangle(x, y_start, x + black_key_width, y_start + black_key_height, BLACK);
            // DrawRectangle(x, y_start, black_key_width, black_key_height, BLACK);
            }
    }

    // // DrawRectangleRounded((Rectangle){0, 0, 160, 36}, 0.3f, 6,
    // //                          (Color){80, 130, 220, 255});
}
