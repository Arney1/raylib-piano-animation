#ifndef DRAW_UTILS_H
#define DRAW_UTILS_H

#include "raylib.h"

void DrawRectSmart(float x, float y, float w, float h, Color color);
void DrawRectGradientSmart(float x, float y, float w, float h, Color c1, Color c2);

void DrawSquircleSmart(Rectangle r, Color color, float rad_percent);
void DrawSquircleGradientSmart(Rectangle r, float rad_percent, Color c1, Color c2);

void DrawCircleSmart(float cx, float cy, float radius, Color color);
void DrawCircleGradientSmart(float cx, float cy, float radius, Color c1, Color c2);

#endif
