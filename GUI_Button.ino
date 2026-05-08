#include "Common.h"

void drawButton(const Button& b) {
  M5.Display.fillRect(b.x, b.y, b.w, b.h, TFT_WHITE);
  M5.Display.drawRect(b.x, b.y, b.w, b.h, TFT_BLACK);
  M5.Display.drawRect(b.x + 2, b.y + 2, b.w - 4, b.h - 4, TFT_BLACK);

  String t = b.text;
  int charW = 14;
  int maxChars = (b.w - 20) / charW;
  if ((int)t.length() > maxChars && maxChars > 3) {
    t = t.substring(0, maxChars - 3) + "...";
  }

  int tw = M5.Display.textWidth(t);
  int fontH = 24;
  int tx = b.x + (b.w - tw) / 2;
  int ty = b.y + (b.h - fontH) / 2;
  M5.Display.setCursor(tx, ty);
  M5.Display.print(t);
}

bool buttonHit(const Button& b, int px, int py) {
  return px >= b.x && px < b.x + b.w &&
         py >= b.y && py < b.y + b.h;
}