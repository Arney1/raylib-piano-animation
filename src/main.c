#include "audio/audio.h"
#include "raylib.h"
#include "utils/color_palette.h"

#include "core/app.h"

#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 900
#define FPS 60

int main(void) {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Piano Animation - Raylib");
  audio_init();
  SetTargetFPS(FPS);

  App_Init();

  while (!WindowShouldClose()) {
    App_Update();
    audio_update();
    BeginDrawing();

    App_Draw();

    EndDrawing();
  }

  App_Unload();
  CloseWindow();

  return 0;
}
