#include "../graphics/bresellipse.h"
#include "../graphics/bresenham.h"
#include "../render/render_state.h"
#include "note_particle.h"
#include "raylib.h"
#include <math.h>

static NoteParticle particles[MAX_PARTICLES];

void particle_system_init(void) {
  for (int i = 0; i < MAX_PARTICLES; i++) {
    particles[i].active = false;
  }
}

void particle_spawn(float x, float y, Color color) {
  for (int i = 0; i < MAX_PARTICLES; i++) {
    if (!particles[i].active) {
      particles[i].active = true;
      particles[i].startX = x;
      particles[i].startY = y;
      particles[i].x = x;
      particles[i].y = y;
      particles[i].lifetime = 0.0f;
      particles[i].maxLife = 1.5f + (GetRandomValue(-2, 2) * 0.1f);
      particles[i].scale = 0.0f;
      particles[i].color = color;
      particles[i].waveOffset = GetRandomValue(0, 314) / 100.0f;
      break;
    }
  }
}

void particle_system_update(void) {
  float dt = GetFrameTime();

  for (int i = 0; i < MAX_PARTICLES; i++) {
    if (!particles[i].active)
      continue;

    NoteParticle *p = &particles[i];
    p->lifetime += dt;

    if (p->lifetime >= p->maxLife) {
      p->active = false;
      continue;
    }

    float lifeRatio = p->lifetime / p->maxLife;
    p->scale = sinf(lifeRatio * PI);

    float floatSpeed = 100.0f;
    float waveFreq = 5.0f;
    float waveAmp = 20.0f;

    p->y = p->startY - (lifeRatio * floatSpeed);

    p->x = p->startX + sinf((p->lifetime * waveFreq) + p->waveOffset) * waveAmp;
  }
}

void particle_system_draw(void) {
  for (int i = 0; i < MAX_PARTICLES; i++) {
    if (!particles[i].active)
      continue;

    NoteParticle *p = &particles[i];

    int base_rx = 8;
    int base_ry = 6;
    int base_stem = 24;

    int rx = (int)(base_rx * p->scale);
    int ry = (int)(base_ry * p->scale);
    int stem_h = (int)(base_stem * p->scale);
    int flag_w = (int)(8 * p->scale);
    int flag_h = (int)(10 * p->scale);

    int cx = (int)p->x;
    int cy = (int)p->y;

    Color pColor = p->color;
    pColor.a = (unsigned char)(255 * p->scale);

    if (gRenderMode == RENDER_FILLED) {
      DrawEllipse(cx, cy, rx, ry, pColor);
      DrawRectangle(cx + rx - 2, cy - stem_h, 2, stem_h, pColor);

      Vector2 flagStart = {cx + rx, cy - stem_h};
      Vector2 flagEnd = {cx + rx + flag_w, cy - stem_h + flag_h};
      DrawLineEx(flagStart, flagEnd, 2.0f, pColor);

    } else {
      BresenhamEllipse(cx, cy, rx, ry, pColor);
      BresenhamLine(cx + rx, cy, cx + rx, cy - stem_h, pColor);
      BresenhamLine(cx + rx, cy - stem_h, cx + rx + flag_w,
                    cy - stem_h + flag_h, pColor);
    }
  }
}

void particle_draw_static(float cx, float cy, Color color) {
  int rx = 16;
  int ry = 12;
  int stem_h = 48;
  int flag_w = 16;
  int flag_h = 20;

  if (gRenderMode == RENDER_FILLED) {
    DrawEllipse((int)cx, (int)cy, rx, ry, color);
    DrawRectangle((int)cx + rx - 2, (int)cy - stem_h, 2, stem_h, color);

    Vector2 flagStart = {cx + rx, cy - stem_h};
    Vector2 flagEnd = {cx + rx + flag_w, cy - stem_h + flag_h};
    DrawLineEx(flagStart, flagEnd, 2.0f, color);
  } else {
    BresenhamEllipse((int)cx, (int)cy, rx, ry, color);
    BresenhamLine((int)cx + rx, (int)cy, (int)cx + rx, (int)cy - stem_h, color);
    BresenhamLine((int)cx + rx, (int)cy - stem_h, (int)cx + rx + flag_w,
                  (int)cy - stem_h + flag_h, color);
  }
}
