#include "raylib.h"
#include "utils/color_palette.h"

#include "core/app.h"

#define SCREEN_WIDTH  1600
#define SCREEN_HEIGHT 900
#define FPS 60

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Piano Animation - Raylib");
    SetTargetFPS(FPS);

    App_Init();

    while (!WindowShouldClose())
    {
        App_Update();

        BeginDrawing();

        App_Draw();

        EndDrawing();
    }

    App_Unload();
    CloseWindow();

    return 0;
}
