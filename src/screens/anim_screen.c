#include "../audio/audio.h"
#include "../graphics/bresenham.h"
#include "../graphics/midcircle.h"
#include "../objects/midi.h"
#include "../objects/note_block.h"
#include "../objects/piano.h"
#include "../render/render_state.h"
#include "../utils/color_palette.h"
#include "../utils/screen_types.h"
#include "anim_screen.h"
#include "raylib.h"
#include <stdbool.h>
#include <stdlib.h>

#define MAX_NOTES 1024
#define FALL_SPEED 200.0f

static NoteBlock notes[MAX_NOTES];
static Piano piano;
static float elapsed_music_time = 0.0f;
static float total_music_time = 1.0f;

void get_key_from_pitch(int pitch, int *key_index, bool *is_black) {
  int octave = pitch / 12;
  int note = pitch % 12; // 0=C, 1=C#, 2=D, 3=D#, 4=E, etc.

  // map the 12 semi-tones to their respective index offsets in the arrays
  int white_offsets[] = {0, -1, 1, -1, 2, 3, -1, 4, -1, 5, -1, 6};
  int black_offsets[] = {-1, 0, -1, 1, -1, -1, 2, -1, 3, -1, 4, -1};

  if (white_offsets[note] != -1) {
    *is_black = false;
    *key_index = (octave * 7) + white_offsets[note];
  } else {
    *is_black = true;
    *key_index = (octave * 5) + black_offsets[note];
  }
}

void spawn_note_from_pitch(int pitch, float duration, float time_until_hit) {
  int key_index;
  bool is_black;

  get_key_from_pitch(pitch, &key_index, &is_black);

  for (int i = 0; i < MAX_NOTES; i++) {
    if (!notes[i].active) {

      if (!is_black && key_index >= piano.white_count)
        break;
      if (is_black && key_index >= piano.black_count)
        break;

      Rectangle r =
          is_black ? piano.black_keys[key_index] : piano.white_keys[key_index];

      // scale height to match the falling speed
      float h = duration * FALL_SPEED;
      if (h < 10.0f)
        h = 10.0f; // short notes visible

      float piano_y = SCREEN_H * 3.0f / 4.0f;
      float distance = time_until_hit * FALL_SPEED;

      // calculate start position so the bottom edge hits the piano exactly on
      // time
      float spawn_y = piano_y - distance - h;

      note_init(&notes[i], r.x, spawn_y, r.width, h, key_index, is_black);
      break;
    }
  }
}

void anim_screen_init(void) {
  midi_load("assets/midis/output.csv");
  midi_reset();
  total_music_time = midi_get_length();
  elapsed_music_time = -5.0f;

  piano_init(&piano, (Vector2){0, SCREEN_H * 3 / 4},
             (Vector2){SCREEN_W - 1, SCREEN_H - 1});

  for (int i = 0; i < MAX_NOTES; i++) {
    notes[i].active = false;
  }
}

void anim_screen_update(void) {
  Rectangle slider_rect = {50, 30, SCREEN_W - 100, 20};
  Vector2 mouse = GetMousePosition();
  if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && mouse.y >= slider_rect.y - 10 &&
      mouse.y <= slider_rect.y + slider_rect.height + 10) {

    // calculate where they clicked percentage-wise
    float progress = (mouse.x - slider_rect.x) / slider_rect.width;
    if (progress < 0.0f)
      progress = 0.0f;
    if (progress > 1.0f)
      progress = 1.0f;

    elapsed_music_time = progress * total_music_time;

    midi_seek(elapsed_music_time);

    // kill all current active notes
    for (int i = 0; i < MAX_NOTES; i++) {
      if (notes[i].active) {
        if (notes[i].triggered) {
          audio_stop_note(notes[i].key_index, notes[i].is_black);
          if (notes[i].is_black)
            piano_set_black(&piano, notes[i].key_index, false);
          else
            piano_set_white(&piano, notes[i].key_index, false);
        }
        notes[i].active = false;
      }
    }
  } else {

    elapsed_music_time += GetFrameTime();
  }
  midi_update(elapsed_music_time);
  if (IsKeyPressed(KEY_O)) {
    gRenderMode =
        (gRenderMode == RENDER_FILLED) ? RENDER_OUTLINE : RENDER_FILLED;
  }

  float piano_y = SCREEN_H * 3 / 4;

  // move notes and stop dead ones
  for (int i = 0; i < MAX_NOTES; i++) {
    NoteBlock *n = &notes[i];

    if (!n->active)
      continue;

    note_update(n);

    float bottom = n->y;

    if (n->triggered && bottom >= piano_y) {
      if (n->is_black)
        piano_set_black(&piano, n->key_index, false);
      else
        piano_set_white(&piano, n->key_index, false);

      audio_stop_note(n->key_index, n->is_black);
      n->active = false;
    }
  }

  // then play notes, ensures a brand new note wont be stopped by an old note
  for (int i = 0; i < MAX_NOTES; i++) {
    NoteBlock *n = &notes[i];

    if (!n->active)
      continue;

    float top = n->y + n->height;

    if (!n->triggered && top >= piano_y) {
      if (n->is_black)
        piano_set_black(&piano, n->key_index, true);
      else
        piano_set_white(&piano, n->key_index, true);

      audio_play_note(n->key_index, n->is_black);
      n->triggered = true;
    }
  }
}

void anim_screen_draw(void) {
  ClearBackground(COLOR_BASE);
  for (int i = 0; i < MAX_NOTES; i++) {
    note_draw(&notes[i]);
  }
  piano_draw(&piano);
  Rectangle slider_track = {50, 30, SCREEN_W - 100, 10};
  DrawRectangleRec(slider_track, DARKGRAY);

  float progress = 0.0f;
  if (total_music_time > 0) {
    progress = elapsed_music_time / total_music_time;
  }
  if (progress < 0.0f)
    progress = 0.0f;
  if (progress > 1.0f)
    progress = 1.0f;

  Rectangle slider_fill = {slider_track.x, slider_track.y,
                           slider_track.width * progress, slider_track.height};
  DrawRectangleRec(slider_fill, LIGHTGRAY);

  // slider thumb
  DrawCircle(slider_fill.x + slider_fill.width,
             slider_fill.y + slider_fill.height / 2, 8, WHITE);
}

void anim_screen_unload(void) { piano_clear(&piano); }
