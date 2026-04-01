#include "../render/render_state.h"
#include "draw_utils.h"

void DrawRectSmart(float x, float y, float w, float h, Color color) {
  if (gRenderMode == RENDER_FILLED) {
    DrawRectangle(x, y, w, h, color);
  } else {
    DrawRectangleLines(x, y, w, h, color);
  }
}

void DrawRectGradientSmart(float x, float y, float w, float h, Color c1,
                           Color c2) {
  if (gRenderMode == RENDER_FILLED) {
    DrawRectangleGradientV(x, y, w, h, c1, c2);
  } else {
    DrawRectangleLines(x, y, w, h, c1);
  }
}
