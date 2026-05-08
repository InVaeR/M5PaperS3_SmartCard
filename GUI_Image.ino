#include "Common.h"

void showImage(const String& path) {
  currentScreen = SCR_IMAGE;
  M5.Display.setEpdMode(epd_quality);
  M5.Display.setRotation(imageRotation);
  M5.Display.clear(TFT_WHITE);

  bool ok = M5.Display.drawPngFile(SD, path.c_str(),
              0, 0,
              M5.Display.width(), M5.Display.height(),
              0, 0,
              0.0f, 0.0f,
              datum_t::middle_center);

  if (!ok) {
    M5.Display.setCursor(20, 40);
    M5.Display.printf("Cannot open:\n%s", path.c_str());
  }

  buttonCount = 0;
  menuButtons[0] = { 0, 0,
    (uint16_t)M5.Display.width(),
    (uint16_t)M5.Display.height(),
    "", "", hMain };
  buttonCount = 1;

  if (showDebug) {
    M5.Display.fillRect(0, 0, M5.Display.width(), TOP_BAR_H, TFT_WHITE);
    M5.Display.setCursor(4, 4);
    M5.Display.printf("File: %s", extractFileName(path).c_str());
  }

  M5.Display.setEpdMode(epd_fast);
}