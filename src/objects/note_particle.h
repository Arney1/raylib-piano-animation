#ifndef NOTE_PARTICLE_H
#define NOTE_PARTICLE_H

#include "raylib.h"
#include <stdbool.h>

#define MAX_PARTICLES 128

typedef struct {
    float x;
    float y;
    float startX;
    float startY;

    float lifetime;     // how long the particle has been alive
    float maxLife;      // total time the particle should live
    float scale;        // current size multiplier (0.0 to 1.0)

    float waveOffset;   // to make each particle wiggle differently
    Color color;
    bool active;
} NoteParticle;

void particle_system_init(void);
void particle_system_update(void);
void particle_system_draw(void);
void particle_spawn(float x, float y, Color color);
void particle_draw_static(float cx, float cy, Color color);

#endif
