#include "raylib.h"
#include "screen_type.h"
#include "ui/back_button.h"
#include "screens/menu.h"
#include "screens/objects.h"
#include "screens/anim.h"
#include "screens/about.h"

int main(void) {
    InitWindow(SCREEN_W, SCREEN_H, "Grafika Komputer - DDA & Bresenham Line");
    SetTargetFPS(60);

    Screen current = MENU;

    while (!WindowShouldClose()) {
        if (current == MENU) {
            if (IsKeyPressed(KEY_ONE)   || IsKeyPressed(KEY_KP_1)) current = ANIM_SCREEN;
            if (IsKeyPressed(KEY_TWO)   || IsKeyPressed(KEY_KP_2)) current = OBJECTS_SCREEN;
            if (IsKeyPressed(KEY_THREE) || IsKeyPressed(KEY_KP_3)) current = ABOUT_SCREEN;
            if (IsKeyPressed(KEY_A)) current = ABOUT;

            // Handle mouse click on program cards
            int clicked = GetClickedProgram();
            if (clicked == 1) current = ANIM_SCREEN;
            if (clicked == 2) current = OBJECTS_SCREEN;
            if (clicked == 3) current = ABOUT_SCREEN;

        }
        if (current != MENU && BackButtonPressed()) current = MENU;

        BeginDrawing();
        switch (current) {
            case MENU:     DrawMenu();     break;
            case ANIM_SCREEN: DrawAnimScreen(); break;
            case OBJECTS_SCREEN: DrawObjectsScreen(); break;
            case ABOUT_SCREEN: DrawAboutScreen(); break;
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
