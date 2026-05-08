#include "Common.h"

int  menuRotation  = 0;
int  imageRotation = 0;
bool showDebug     = false;

Screen currentScreen   = SCR_MAIN;
int    currentMenuPage = 0;

Button menuButtons[MAX_BUTTONS];
int    buttonCount = 0;

String pngFiles[MAX_PNG_FILES];
int    pngFileCount = 0;

uint32_t lastBatteryUpdate = 0;