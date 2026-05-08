#include "Common.h"

void startDisplay() {
  M5.Display.setEpdMode(epd_fast);
  M5.Display.setFont(&fonts::FreeMono12pt7b);
  M5.Display.setRotation(menuRotation);
  M5.Display.setTextColor(TFT_BLACK, TFT_WHITE);
  M5.Display.clear(TFT_WHITE);
}

void drawTopBar() {
  bool charging = M5.Power.isCharging();
  int level = M5.Power.getBatteryLevel();
  int volt  = M5.Power.getBatteryVoltage();

  int W = M5.Display.width();
  M5.Display.fillRect(0, 0, W, TOP_BAR_H, TFT_WHITE);
  M5.Display.setCursor(4, 4);
  M5.Display.printf("%s%3d%% %4dmV  PNG:%d  P:%d",
    charging ? "[C] " : "    ",
    level, volt, pngFileCount, currentMenuPage + 1);
  M5.Display.drawFastHLine(0, TOP_BAR_H, W, TFT_BLACK);
}

void beep() { M5.Speaker.tone(BEEP_FREQ_HZ, BEEP_DUR_MS); }

bool menuAddButton(const String& text, FuncHandler h, const String& data) {
  if (buttonCount >= MAX_BUTTONS) return false;
  menuButtons[buttonCount++] = { 0, 0, 0, 0, text, data, h };
  return true;
}

void layoutButtons() {
  int W = M5.Display.width();
  int H = M5.Display.height();

  int areaTop = BTN_MARGIN_TOP;
  int areaH   = H - areaTop - 10;

  int totalH = buttonCount * BTN_HEIGHT + (buttonCount - 1) * BTN_GAP;
  int startY = (totalH < areaH) ? areaTop + (areaH - totalH) / 2 : areaTop;

  int x = BTN_MARGIN_X;
  int w = W - 2 * BTN_MARGIN_X;

  for (int i = 0; i < buttonCount; i++) {
    menuButtons[i].x = x;
    menuButtons[i].y = startY + i * (BTN_HEIGHT + BTN_GAP);
    menuButtons[i].w = w;
    menuButtons[i].h = BTN_HEIGHT;
  }
}

void drawMenu() {
  M5.Display.clear(TFT_WHITE);
  drawTopBar();
  for (int i = 0; i < buttonCount; i++) drawButton(menuButtons[i]);
}

// ===== Handlers =====
void hMain(Button*)     { currentMenuPage = 0; mainMenuInit(); }
void hSettings(Button*) { settingsMenuInit(); }
void hRescan(Button*)   { scanForPNGFiles(); currentMenuPage = 0; mainMenuInit(); }

void hNext(Button*) {
  int total = (pngFileCount + FILES_PER_PAGE - 1) / FILES_PER_PAGE;
  if (total == 0) total = 1;
  if (currentMenuPage < total - 1) currentMenuPage++;
  mainMenuInit();
}
void hPrev(Button*) {
  if (currentMenuPage > 0) currentMenuPage--;
  mainMenuInit();
}

void hRotateMenu(Button*) {
  menuRotation = (menuRotation + 1) % 4;
  saveSettings();
  M5.Display.setRotation(menuRotation);
  settingsMenuInit();
}
void hRotateImage(Button*) {
  imageRotation = (imageRotation + 1) % 4;
  saveSettings();
  settingsMenuInit();
}
void hToggleDebug(Button*) {
  showDebug = !showDebug;
  saveSettings();
  settingsMenuInit();
}
void hSelectFile(Button* b) {
  showImage(b->data.length() ? b->data : b->text);
}

// ===== Screens =====
void mainMenuInit() {
  currentScreen = SCR_MAIN;
  M5.Display.setRotation(menuRotation);
  buttonCount = 0;

  if (currentMenuPage == 0) menuAddButton("Settings", hSettings);
  else                      menuAddButton("< Prev",   hPrev);

  int start = currentMenuPage * FILES_PER_PAGE;
  int endIdx = min(start + FILES_PER_PAGE, pngFileCount);
  for (int i = start; i < endIdx; i++) {
    menuAddButton(extractFileName(pngFiles[i]), hSelectFile, pngFiles[i]);
  }

  if (pngFileCount == 0) menuAddButton("No PNG. Insert SD & rescan", hRescan);

  int total = (pngFileCount + FILES_PER_PAGE - 1) / FILES_PER_PAGE;
  if (currentMenuPage < total - 1) menuAddButton("Next >", hNext);
  else                             menuAddButton("Rescan", hRescan);

  layoutButtons();
  drawMenu();
}

void settingsMenuInit() {
  currentScreen = SCR_SETTINGS;
  M5.Display.setRotation(menuRotation);
  buttonCount = 0;

  menuAddButton(String("Menu rotation: ")  + menuRotation,  hRotateMenu);
  menuAddButton(String("Image rotation: ") + imageRotation, hRotateImage);
  menuAddButton(String("Debug info: ") + (showDebug ? "ON" : "OFF"), hToggleDebug);
  menuAddButton("Rescan SD",      hRescan);
  menuAddButton("< Back to menu", hMain);

  layoutButtons();
  drawMenu();
}

void handleTouch(int x, int y) {
  for (int i = 0; i < buttonCount; i++) {
    if (buttonHit(menuButtons[i], x, y)) {
      if (menuButtons[i].handler) {
        beep();
        menuButtons[i].handler(&menuButtons[i]);
      }
      return;
    }
  }
}

void guiUpdate() {
  auto t = M5.Touch.getDetail();
  if (t.wasClicked()) handleTouch(t.x, t.y);

  if (currentScreen == SCR_MAIN && t.wasFlicked()) {
    int dx = t.distanceX();
    int dy = t.distanceY();
    if (abs(dx) > abs(dy) && abs(dx) > FLICK_THRESHOLD) {
      if (dx < 0) hNext(nullptr);
      else        hPrev(nullptr);
    }
  }

  if (currentScreen != SCR_IMAGE) {
    uint32_t now = millis();
    if (now - lastBatteryUpdate > BATTERY_UPDATE_MS) {
      drawTopBar();
      lastBatteryUpdate = now;
    }
  }
}