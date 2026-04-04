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
  int cardH = 420;
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

  int sumY = infoY + 90;
  int sumW = 130;
  int sumH = 50;
  int sumGap = 10;
  int sumStartX = SCREEN_W / 2 - (4 * sumW + 3 * sumGap) / 2;

  const char *pLabels[] = {"MIDI Parser", "Piano UI", "Anim Loop",
                           "Squircle UI"};
  Color pColors[] = {SKYBLUE, VIOLET, GREEN, ORANGE};

  for (int i = 0; i < 4; i++) {
    int bx = sumStartX + i * (sumW + sumGap);
    Rectangle badgeRec = {bx, sumY, sumW, sumH};

    Color badgeBg = pColors[i];
    badgeBg.a = 70;

    DrawSquircleSmart(badgeRec, badgeBg, 0.3f);

    int labelWidth = MeasureText(pLabels[i], 16);
    DrawText(pLabels[i], bx + (sumW / 2) - (labelWidth / 2), sumY + 17, 16,
             pColors[i]);
  }
}

void about_screen_unload(void) { is_about_active = false; }
