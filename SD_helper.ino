#include "Common.h"
#include <SPI.h>

static void collectPNG(File dir, const String& parent, int depth) {
  if (depth > MAX_SCAN_DEPTH) return;
  while (pngFileCount < MAX_PNG_FILES) {
    File entry = dir.openNextFile();
    if (!entry) break;

    String name = entry.name();
    int s = name.lastIndexOf('/');
    if (s >= 0) name = name.substring(s + 1);
    String full = parent + name;

    if (entry.isDirectory()) {
      collectPNG(entry, full + "/", depth + 1);
    } else if (lower(name).endsWith(".png")) {
      pngFiles[pngFileCount++] = full;
    }
    entry.close();
  }
}

static void sortFiles() {
  for (int i = 1; i < pngFileCount; i++) {
    String key = pngFiles[i];
    String keyL = lower(key);
    int j = i - 1;
    while (j >= 0 && lower(pngFiles[j]).compareTo(keyL) > 0) {
      pngFiles[j+1] = pngFiles[j];
      j--;
    }
    pngFiles[j+1] = key;
  }
}

void scanForPNGFiles() {
  pngFileCount = 0;
  File root = SD.open("/");
  if (root) { collectPNG(root, "/", 0); root.close(); }
  sortFiles();
}

void startSdCard() {
  SPI.begin(SD_SCK_PIN, SD_MISO_PIN, SD_MOSI_PIN, SD_CS_PIN);

  M5.Display.setCursor(0, 30);
  M5.Display.println("Mounting SD...");

  bool ok = false;
  for (int i = 0; i < 3 && !ok; i++) {
    ok = SD.begin(SD_CS_PIN, SPI, SD_FREQ);
    if (!ok) delay(300);
  }

  if (!ok) {
    M5.Display.println("SD not detected!");
    delay(1500);
    return;
  }

  M5.Display.println("SD OK. Scanning...");
  scanForPNGFiles();
  M5.Display.printf("Found %d PNG files\n", pngFileCount);
  delay(500);
}