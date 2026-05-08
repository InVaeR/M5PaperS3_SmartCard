#include "Common.h"
#include <Preferences.h>

void loadSettings() {
  Preferences p;
  p.begin("smartcard", true);
  menuRotation  = p.getInt ("menuRot", 0);
  imageRotation = p.getInt ("imgRot",  0);
  showDebug     = p.getBool("debug", false);
  p.end();
}

void saveSettings() {
  Preferences p;
  p.begin("smartcard", false);
  p.putInt ("menuRot", menuRotation);
  p.putInt ("imgRot",  imageRotation);
  p.putBool("debug",   showDebug);
  p.end();
}

void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);

  loadSettings();
  startDisplay();
  startSdCard();
  M5.Speaker.setVolume(SPEAKER_VOLUME);

  mainMenuInit();
}

void loop() {
  M5.update();
  guiUpdate();
  delay(10);
}