#ifndef DRAW_UTILS_H
#define DRAW_UTILS_H

#include "raylib.h"

void DrawRectSmart(float x, float y, float w, float h, Color color);
void DrawSquircleSmart(Rectangle r, Color color, float rad_percent);
void DrawRectGradientSmart(float x, float y, float w, float h, Color c1, Color c2);

#endif
