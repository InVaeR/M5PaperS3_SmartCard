#pragma once
#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <M5Unified.h>
#include <M5GFX.h>

// ===== SD pins =====
#define SD_CS_PIN    47
#define SD_SCK_PIN   39
#define SD_MOSI_PIN  38
#define SD_MISO_PIN  40
#define SD_FREQ      25000000

// ===== Limits =====
#define MAX_PNG_FILES   100
#define FILES_PER_PAGE  8
#define MAX_BUTTONS     12
#define MAX_SCAN_DEPTH  5

// ===== GUI layout =====
#define TOP_BAR_H       28
#define BTN_HEIGHT      80
#define BTN_GAP         8
#define BTN_MARGIN_X    12
#define BTN_MARGIN_TOP  36

// ===== Behavior =====
#define BATTERY_UPDATE_MS  60000
#define FLICK_THRESHOLD    50
#define BEEP_FREQ_HZ       1180
#define BEEP_DUR_MS        30
#define SPEAKER_VOLUME     180

// ===== Types =====
struct Button;
typedef void (*FuncHandler)(Button*);

struct Button {
  int x, y;
  uint16_t w, h;
  String text;
  String data;
  FuncHandler handler;
};

enum Screen { SCR_MAIN, SCR_SETTINGS, SCR_IMAGE };

// ===== Globals =====
extern int  menuRotation;
extern int  imageRotation;
extern bool showDebug;

extern Screen currentScreen;
extern int    currentMenuPage;

extern Button menuButtons[MAX_BUTTONS];
extern int    buttonCount;

extern String pngFiles[MAX_PNG_FILES];
extern int    pngFileCount;

extern uint32_t lastBatteryUpdate;

// ===== Prototypes =====
void loadSettings();
void saveSettings();

String extractFileName(const String& fullPath, bool stripExt = true);
String lower(const String& s);

void startSdCard();
void scanForPNGFiles();

void drawButton(const Button& b);
bool buttonHit(const Button& b, int px, int py);

void startDisplay();
void drawTopBar();
void beep();
bool menuAddButton(const String& text, FuncHandler h, const String& data = "");
void layoutButtons();
void drawMenu();
void mainMenuInit();
void settingsMenuInit();
void handleTouch(int x, int y);
void guiUpdate();

void showImage(const String& path);

void hMain(Button*);
void hSettings(Button*);
void hRescan(Button*);
void hNext(Button*);
void hPrev(Button*);
void hRotateMenu(Button*);
void hRotateImage(Button*);
void hToggleDebug(Button*);
void hSelectFile(Button*);