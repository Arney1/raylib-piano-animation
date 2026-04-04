#include "../audio/audio.h"
#include "../graphics/bresenham.h"
#include "../graphics/midcircle.h"
#include "../objects/midi.h"
#include "../objects/midi_slider.h"
#include "../objects/note_block.h"
#include "../objects/note_particle.h"
#include "../objects/piano.h"
#include "../render/render_state.h"
#include "../utils/color_palette.h"
#include "../utils/draw_utils.h"
#include "../utils/screen_types.h"
#include "anim_screen.h"
#include "menu_screen.h"
#include "raylib.h"
#include <stdbool.h>
#include <stdlib.h>

#define MAX_NOTES 1024
#define FALL_SPEED 225.0f

static NoteBlock notes[MAX_NOTES];
static Piano piano;
static float elapsed_music_time = 0.0f;
static float total_music_time = 1.0f;
static bool is_anim_active = false;

void get_key_from_pitch(int pitch, int *key_index, bool *is_black) {
  int octave = pitch / 12;
  int note = pitch % 12; // 0=C, 1=C#, 2=D, 3=D#, 4=E, etc.

  // map the 12 semi-tones to the respective index offsets in the arrays
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
  is_anim_active = true;
  midi_load("assets/midis/output.csv");
  midi_reset();
  total_music_time = midi_get_length();
  elapsed_music_time = -5.0f;

  piano_init(&piano, (Vector2){0, SCREEN_H * 3 / 4},
             (Vector2){SCREEN_W - 1, SCREEN_H - 1});

  for (int i = 0; i < MAX_NOTES; i++) {
    notes[i].active = false;
  }
  particle_system_init();
}

void anim_screen_update(Screen *currentScreen) {
  if (IsKeyPressed(KEY_BACKSPACE)) {
    for (int i = 0; i < MAX_NOTES; i++) {
      if (notes[i].active && notes[i].triggered) {
        audio_stop_note(notes[i].key_index, notes[i].is_black);
      }
    }

    *currentScreen = SCREEN_MENU;
    anim_screen_unload();
    menu_screen_init();
    return;
  }

  particle_system_update();

  Rectangle slider_track = {50, 70, SCREEN_W - 100, 10};
  bool user_is_seeking =
      midi_slider_update(slider_track, &elapsed_music_time, total_music_time);

  if (user_is_seeking) {
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

    // delete notes
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

      Color floatingNoteCol =
          n->is_black ? (Color){0, 255, 255, 255} : (Color){255, 255, 0, 255};

      // floating musical note particle spawn
      particle_spawn(n->x + (n->width / 2.0f), piano_y, floatingNoteCol);
    }
  }
}

void anim_screen_draw(void) {
  if (!is_anim_active)
    return;
  ClearBackground(COLOR_BASE);
  for (int i = 0; i < MAX_NOTES; i++) {
    note_draw(&notes[i]);
  }
  piano_draw(&piano);
  particle_system_draw();
  Rectangle slider_track = {50, 70, SCREEN_W - 100, 10};
  midi_slider_draw(slider_track, elapsed_music_time, total_music_time);

  const char *hints = "[Backspace] : Back  |  [Esc] : Exit  |  [O] : Outline  "
                      "|  [<-] / [->] : Seek";
  int fontSize = 20;
  int textWidth = MeasureText(hints, fontSize);
  int padY = 12;

  Rectangle hintBg = {50.0f, 20.0f, SCREEN_W - 100.0f,
                      fontSize + (padY * 2.0f)};

  Color bgCol = COLOR_SURFACE;
  bgCol.a = 200;

  DrawSquircleSmart(hintBg, bgCol, 0.4f);

  float textX = hintBg.x + (hintBg.width / 2.0f) - (textWidth / 2.0f);

  DrawText(hints, (int)textX, (int)(hintBg.y + padY), fontSize, COLOR_TEXT);
}

void anim_screen_unload(void) {
  is_anim_active = false;
  piano_clear(&piano);
}
