#include "../objects/midi_slider.h"
#include "../objects/note_block.h"
#include "../objects/note_particle.h"
#include "../objects/piano.h"
#include "../render/render_state.h"
#include "../utils/color_palette.h"
#include "../utils/draw_utils.h"
#include "../utils/screen_types.h"
#include "menu_screen.h"
#include "objects_screen.h"
#include "raylib.h"
#include <stdbool.h>

static int current_object_index = 0;
static const int NUM_OBJECTS = 4;
static const char *object_names[] = {"1. Piano", "2. Note Block",
                                     "3. MIDI Slider", "4. Note Particle"};

static bool is_objects_active = false;
static bool animations_enabled = false;

static Piano obj_piano;
static NoteBlock obj_note;

static float piano_timer = 0.0f;
static int last_pressed_white = -1;
static int last_pressed_black = -1;

static float slider_elapsed = 0.0f;
static float slider_total = 5.0f;

static float particle_timer = 0.0f;

// centralized card boundaries
static Rectangle get_card_rect(void) {
  return (Rectangle){100, 100, SCREEN_W - 200, SCREEN_H - 150};
}

void objects_screen_init(void) {
  is_objects_active = true;
  current_object_index = 0;
  animations_enabled = false;

  Rectangle cardRec = get_card_rect();

  piano_init(&obj_piano,
             (Vector2){cardRec.x + 40, cardRec.y + cardRec.height / 2.0f - 60},
             (Vector2){cardRec.x + cardRec.width - 40,
                       cardRec.y + cardRec.height / 2.0f + 60});

  note_init(&obj_note, SCREEN_W / 2.0f - 20, cardRec.y + 40, 40, 100, 0, false);
  obj_note.active = true;

  slider_elapsed = 0.0f;
  particle_system_init();
}

void objects_screen_update(Screen *currentScreen) {
  if (IsKeyPressed(KEY_BACKSPACE)) {
    *currentScreen = SCREEN_MENU;
    objects_screen_unload();
    menu_screen_init();
    return;
  }

  if (IsKeyPressed(KEY_O)) {
    gRenderMode =
        (gRenderMode == RENDER_FILLED) ? RENDER_OUTLINE : RENDER_FILLED;
  }

  if (IsKeyPressed(KEY_RIGHT)) {
    current_object_index = (current_object_index + 1) % NUM_OBJECTS;
  }
  if (IsKeyPressed(KEY_LEFT)) {
    current_object_index =
        (current_object_index - 1 + NUM_OBJECTS) % NUM_OBJECTS;
  }

  Rectangle cardRec = get_card_rect();

  if (IsKeyPressed(KEY_SPACE)) {
    animations_enabled = !animations_enabled;

    if (!animations_enabled) {
      if (last_pressed_white != -1)
        piano_set_white(&obj_piano, last_pressed_white, false);
      if (last_pressed_black != -1)
        piano_set_black(&obj_piano, last_pressed_black, false);
      last_pressed_white = -1;
      last_pressed_black = -1;

      slider_elapsed = slider_total / 2.0f;
      obj_note.y = cardRec.y + 40;
      particle_system_init();
    }
  }
  particle_system_update();
  if (animations_enabled) {
    switch (current_object_index) {
    case 0:
      piano_timer += GetFrameTime();
      if (piano_timer > 0.15f) {
        piano_timer = 0.0f;
        if (last_pressed_white != -1)
          piano_set_white(&obj_piano, last_pressed_white, false);
        if (last_pressed_black != -1)
          piano_set_black(&obj_piano, last_pressed_black, false);

        last_pressed_white = GetRandomValue(0, obj_piano.white_count - 1);
        last_pressed_black = GetRandomValue(0, obj_piano.black_count - 1);
        piano_set_white(&obj_piano, last_pressed_white, true);
        piano_set_black(&obj_piano, last_pressed_black, true);
      }
      break;

    case 1:
      obj_note.y += 300.0f * GetFrameTime();
      if (obj_note.y > cardRec.y + cardRec.height - 120) {
        obj_note.y = cardRec.y + 40;
      }
      note_update(&obj_note);
      break;

    case 2:
      slider_elapsed += GetFrameTime();
      if (slider_elapsed > slider_total) {
        slider_elapsed = 0.0f;
      }
      break;
    case 3: // note particle
      particle_timer += GetFrameTime();
      if (particle_timer > 0.15f) { // spawn a new batch every 0.15s
        particle_timer = 0.0f;
        float spawnX = cardRec.x + cardRec.width / 2.0f;
        float spawnY = cardRec.y + cardRec.height / 2.0f;
        particle_spawn(spawnX, spawnY, SKYBLUE);
        particle_spawn(spawnX, spawnY, YELLOW);
      }
      break;
    }
  } else {
  }
}

void objects_screen_draw(void) {
  if (!is_objects_active)
    return;

  ClearBackground(COLOR_BASE);

  const char *hints =
      "[Backspace] : Back  |  [Esc] : Exit  |  [O] : Outline  |  "
      "[<-]/[->] : Switch  |  "
      "[Space] : Toggle Anim";
  int fontSize = 20;
  int padY = 12;

  Rectangle hintBg = {50.0f, 20.0f, SCREEN_W - 100.0f,
                      fontSize + (padY * 2.0f)};
  Color bgCol = COLOR_SURFACE;
  bgCol.a = 200;
  DrawSquircleSmart(hintBg, bgCol, 0.4f);

  int textWidth = MeasureText(hints, fontSize);
  float textX = hintBg.x + (hintBg.width / 2.0f) - (textWidth / 2.0f);
  DrawText(hints, (int)textX, (int)(hintBg.y + padY), fontSize, COLOR_TEXT);

  Rectangle cardRec = get_card_rect();
  DrawSquircleSmart(cardRec, COLOR_SURFACE, 0.01f);

  int arrowY = cardRec.y + (cardRec.height / 2) - 20;
  DrawText("<", cardRec.x - 60, arrowY, 40, COLOR_TEXT_MUTED);
  DrawText(">", cardRec.x + cardRec.width + 30, arrowY, 40, COLOR_TEXT_MUTED);

  DrawText(object_names[current_object_index], cardRec.x + 40, cardRec.y + 40,
           30, COLOR_PRIMARY);
  const char *status = animations_enabled ? "Animation: ON" : "Animation: OFF";
  DrawText(status, cardRec.x + 40, cardRec.y + 80, 20,
           animations_enabled ? GREEN : COLOR_TEXT_MUTED);

  switch (current_object_index) {
  case 0:
    piano_draw(&obj_piano);
    break;
  case 1:
    note_draw(&obj_note);
    break;
  case 2: {
    Rectangle slider_rect = {cardRec.x + 80,
                             cardRec.y + (cardRec.height / 2.0f) - 5,
                             cardRec.width - 160, 10};
    midi_slider_draw(slider_rect, slider_elapsed, slider_total);
  } break;
  case 3:
    if (animations_enabled) {
      particle_system_draw();
    } else {
      particle_draw_static(cardRec.x + cardRec.width / 2.0f,
                           cardRec.y + cardRec.height / 2.0f, SKYBLUE);
    }
    break;
  }
}

void objects_screen_unload(void) {
  is_objects_active = false;
  piano_clear(&obj_piano);
}
