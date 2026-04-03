#include "../utils/draw_utils.h"
#include "midi_slider.h"

bool midi_slider_update(Rectangle bounds, float *elapsed_time,
                        float total_time) {
  Vector2 mouse = GetMousePosition();

  // expanded hitbox
  Rectangle hit_rect = {bounds.x, bounds.y - 10, bounds.width,
                        bounds.height + 20};

  if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && mouse.y >= hit_rect.y &&
      mouse.y <= hit_rect.y + hit_rect.height) {

    float progress = (mouse.x - bounds.x) / bounds.width;
    if (progress < 0.0f)
      progress = 0.0f;
    if (progress > 1.0f)
      progress = 1.0f;

    if (total_time > 0.0f) {
      *elapsed_time = progress * total_time;
    }
    return true;
  }

  return false;
}

void midi_slider_draw(Rectangle bounds, float elapsed_time, float total_time) {
  DrawSquircleSmart(bounds, LIGHTGRAY, 0.5f);

  float progress = 0.0f;
  if (total_time > 0.0f) {
    progress = elapsed_time / total_time;
  }
  if (progress < 0.0f)
    progress = 0.0f;
  if (progress > 1.0f)
    progress = 1.0f;

  if (progress > 0.0f) {
    Rectangle slider_fill = {bounds.x, bounds.y, bounds.width * progress,
                             bounds.height};
    DrawSquircleSmart(slider_fill, WHITE, 0.5f);
  }

  // slider thumb
  float cx = bounds.x + (bounds.width * progress);
  float cy = bounds.y + bounds.height / 2.0f;
  DrawCircleSmart(cx, cy, 8.0f, WHITE);
}
