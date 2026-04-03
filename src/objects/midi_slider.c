#include "../utils/draw_utils.h"
#include "midi_slider.h"

static float hold_time = 0.0f;

bool midi_slider_update(Rectangle bounds, float *elapsed_time,
                        float total_time) {
  bool seeking = false;
  float base_skip_speed = 3.0f;

  if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_LEFT)) {
    hold_time += GetFrameTime();

    // exponential
    float current_speed =
        base_skip_speed * (1.0f + (hold_time * hold_time) * 15.0f);
    float delta = current_speed * GetFrameTime();

    if (IsKeyDown(KEY_LEFT)) {
      *elapsed_time -= delta;
    } else {
      *elapsed_time += delta;
    }

    if (*elapsed_time < -5.0f)
      *elapsed_time = -5.0f;
    if (*elapsed_time > total_time)
      *elapsed_time = total_time;

    seeking = true;
  } else {
    hold_time = 0.0f;
  }

  return seeking;
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
