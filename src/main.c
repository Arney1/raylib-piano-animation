#include "raylib.h"
#include "core/app.h"

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600
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
        ClearBackground(RAYWHITE);

        App_Draw();

        EndDrawing();
    }

    App_Unload();
    CloseWindow();

    return 0;
}
