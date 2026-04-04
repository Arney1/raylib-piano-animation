#include "../utils/color_palette.h"
#include "../utils/draw_utils.h"
#include "../utils/screen_types.h"
#include "about_screen.h"
#include "menu_screen.h"
#include "raylib.h"
#include <stdbool.h>

static bool is_about_active = false;

void about_screen_init(void) { is_about_active = true; }

void about_screen_update(Screen *currentScreen) {
  if (IsKeyPressed(KEY_BACKSPACE)) {
    *currentScreen = SCREEN_MENU;
    about_screen_unload();
    menu_screen_init();
  }
}

void about_screen_draw(void) {
  if (!is_about_active)
    return;

  ClearBackground(COLOR_BASE);

  const char *hints = "[Backspace] : Back  |  [Esc] : Exit";
  int hintFontSize = 20;
  int padY = 12;
  int textWidth = MeasureText(hints, hintFontSize);

  Rectangle hintBg = {50.0f, 20.0f, SCREEN_W - 100.0f,
                      hintFontSize + (padY * 2.0f)};
  Color bgCol = COLOR_SURFACE;
  bgCol.a = 200;
  DrawSquircleSmart(hintBg, bgCol, 0.4f);

  float textX = hintBg.x + (hintBg.width / 2.0f) - (textWidth / 2.0f);
  DrawText(hints, (int)textX, (int)(hintBg.y + padY), hintFontSize, COLOR_TEXT);

  int cardW = 640;
  int cardH = 450;
  int cardX = (SCREEN_W / 2) - (cardW / 2);
  int cardY = (SCREEN_H / 2) - (cardH / 2) + 20;

  Rectangle cardRec = {cardX, cardY, cardW, cardH};
  DrawSquircleSmart(cardRec, COLOR_SURFACE, 0.15f);

  int avSize = 100;
  Rectangle avRec = {SCREEN_W / 2.0f - (avSize / 2.0f), cardY + 30, avSize,
                     avSize};
  DrawSquircleSmart(avRec, COLOR_PRIMARY, 0.4f);

  int rWidth = MeasureText("A", 60);
  DrawText("A", avRec.x + (avSize / 2) - (rWidth / 2), avRec.y + 20, 60,
           COLOR_BASE);

  int nameWidth = MeasureText("Arkan Ramadhan Nugraha", 32);
  DrawText("Arkan Ramadhan Nugraha", SCREEN_W / 2 - (nameWidth / 2),
           avRec.y + avSize + 15, 32, COLOR_TEXT);

  int nimWidth = MeasureText("NIM : 241524033", 20);
  DrawText("NIM : 241524033", SCREEN_W / 2 - (nimWidth / 2),
           avRec.y + avSize + 55, 20, COLOR_PRIMARY);

  Rectangle divider = {cardX + 40, avRec.y + avSize + 100, cardW - 80, 2};
  DrawSquircleSmart(divider, COLOR_BASE, 0.5f);

  int infoY = divider.y + 20;
  DrawText("Mata Kuliah : Komputer Grafik", cardX + 40, infoY, 16,
           COLOR_TEXT_MUTED);
  DrawText("Topik : C & Raylib MIDI Visualizer", cardX + 40, infoY + 25, 16,
           COLOR_TEXT_MUTED);
  DrawText("Library : Raylib 5.5", cardX + 40, infoY + 50, 16,
           COLOR_TEXT_MUTED);

  int descY = infoY + 90;
  Rectangle descRec = {cardX + 40, descY, cardW - 80, 80};
  Color descBg = COLOR_BASE;
  DrawSquircleSmart(descRec, descBg, 0.1f);

  const char *line1 = "A MIDI visualizer built in C using Raylib.";
  const char *line2 = "Loads a MIDI CSV, falls notes onto a live piano,";
  const char *line3 = "and plays back audio in real-time with full seek.";

  int l1w = MeasureText(line1, 16);
  int l2w = MeasureText(line2, 16);
  int l3w = MeasureText(line3, 16);

  DrawText(line1, cardX + 40 + (cardW - 80) / 2 - l1w / 2, descY + 14, 16,
           COLOR_TEXT);
  DrawText(line2, cardX + 40 + (cardW - 80) / 2 - l2w / 2, descY + 36, 16,
           COLOR_TEXT_MUTED);
  DrawText(line3, cardX + 40 + (cardW - 80) / 2 - l3w / 2, descY + 56, 16,
           COLOR_TEXT_MUTED);
}

void about_screen_unload(void) { is_about_active = false; }
