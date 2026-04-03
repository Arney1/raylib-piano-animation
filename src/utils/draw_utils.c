#include "../graphics/bresenham.h"
#include "../graphics/midcircle.h"
#include "../render/render_state.h"
#include "draw_utils.h"

void DrawRectSmart(float x, float y, float w, float h, Color color) {
  if (gRenderMode == RENDER_FILLED) {
    DrawRectangle(x, y, w, h, color);
  } else {
    BresenhamRectangle(x, y, x + w, y + h, color);
    // DrawRectangleLines(x, y, w, h, color);
  }
}

void DrawSquircleSmart(Rectangle r, Color color, float rad_percent) {
  float eps = 0.75f;
  float x, y, w, h, rad;
  Vector2 centers[4];
  // rad_percent maximum is 0.5
  if (rad_percent > 0.5f)
    rad_percent = 0.5f;
  if (r.width < r.height) {
    rad = rad_percent * r.width;
    x = r.x;
    y = r.y + rad;
    w = r.width;
    h = r.height - 2 * rad;
    centers[0] = (Vector2){x + rad + eps, y + eps};
    centers[1] = (Vector2){x + w - rad - eps, y + eps};
    centers[3] = (Vector2){x + rad + eps, y + h - eps};
    centers[2] = (Vector2){x + w - rad - eps, y + h - eps};
  } else {
    rad = rad_percent * r.height;
    x = r.x + rad;
    y = r.y;
    w = r.width - 2 * rad;
    h = r.height;
    centers[0] = (Vector2){x + eps, y + rad + eps};
    centers[1] = (Vector2){x - eps, y + h - rad + eps};
    centers[3] = (Vector2){x + w + eps, y + rad - eps};
    centers[2] = (Vector2){x + w - eps, y + h - rad - eps};
  }
  if (gRenderMode == RENDER_FILLED) {
    if (r.width < r.height) {

      DrawRectangle(r.x + rad - eps, r.y, w - 2 * rad + 2 * eps, rad + eps,
                    color);
      DrawRectangle(r.x + rad - eps, y + h - eps, w - 2 * rad + 2 * eps,
                    rad + eps, color);
    } else {
      DrawRectangle(r.x, r.y + rad - eps, rad + eps, h - 2 * rad + 2 * eps,
                    color);

      DrawRectangle(x + w - eps, r.y + rad - eps, rad + eps,
                    h - 2 * rad + 2 * eps, color);
    }
    for (int i = 0; i < 4; i++) {
      DrawCircleSector(centers[i], rad + 0.17f, (i + 2) * 90.0f,
                       (i + 3) * 90.0f, 100, color);
    }
    DrawRectangle(x, y, w, h, color);
  } else {
    rad = rad_percent * ((r.width < r.height) ? r.width : r.height);

    int x0 = (int)r.x;
    int y0 = (int)r.y;
    int x1 = (int)(r.x + r.width);
    int y1 = (int)(r.y + r.height);

    int rint = (int)rad;

    // centers
    int cx0 = x0 + rint;
    int cx1 = x1 - rint;
    int cy0 = y0 + rint;
    int cy1 = y1 - rint;

    // 4 quarter circles
    MidcircleQuarter(cx1, cy0, rint, 0, color); // top-right
    MidcircleQuarter(cx0, cy0, rint, 1, color); // top-left
    MidcircleQuarter(cx0, cy1, rint, 2, color); // bottom-left
    MidcircleQuarter(cx1, cy1, rint, 3, color); // bottom-right

    // 4 straight edges
    BresenhamLine(cx0, y0, cx1, y0, color);
    BresenhamLine(cx0, y1, cx1, y1, color);
    BresenhamLine(x0, cy0, x0, cy1, color);
    BresenhamLine(x1, cy0, x1, cy1, color);
  }
}

void DrawRectGradientSmart(float x, float y, float w, float h, Color c1,
                           Color c2) {
  if (gRenderMode == RENDER_FILLED) {
    DrawRectangleGradientV(x, y, w, h, c1, c2);
  } else {
    Color c;
    c.r = (c1.r + c2.r) / 2;
    c.g = (c1.g + c2.g) / 2;
    c.b = (c1.b + c2.b) / 2;
    c.a = (c1.a + c2.a) / 2;
    // for (int i = 0; i < 4; i++) {
    //   c[i] = (c1[i] + c2[i]) / 2;
    // }
    BresenhamRectangle(x, y, x + w, y + h, c);
  }
}
