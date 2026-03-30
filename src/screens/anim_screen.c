#include "anim_screen.h"
#include "raylib.h"
#include "../utils/color_palette.h"
#include "../graphics/bresenham.h"
#include "../graphics/midcircle.h"
// #include "../graphics/ellipse.h"

static float noteY = 0;

void AnimScreen_Init(void) {
    noteY = 0;
}

void AnimScreen_Update(void) {
    noteY += 2.0f;
}

void AnimScreen_Draw(void) {
    ClearBackground(COLOR_BASE);

    // contoh pake algoritma
    BresenhamRectangle(100, (int)noteY, 200, (int)noteY + 50, (Color){80, 130, 220, 255});
    // BresenhamLine(100, (int)noteY, 100, (int)noteY + 50, (Color){80, 130, 220, 255});
}

void AnimScreen_Unload(void) {

}
