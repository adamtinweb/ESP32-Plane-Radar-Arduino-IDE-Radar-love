#include "display_font.h"

#include "display.h"

// Arduino IDE version:
// PlatformIO embedded the ui_font.vlw file at link time.
// To keep this Arduino port simple, smooth VLW font loading is disabled.
// The UI automatically falls back to built-in LovyanGFX bitmap fonts.

bool displayFontInit() {
  Serial.println("Arduino IDE build: using bitmap font fallback");
  return false;
}

bool displayFontIsSmooth() {
  return false;
}

bool displayFontEnsureLoaded(lgfx::LGFXBase& gfx) {
  (void)gfx;
  return false;
}

void displayFontSetSmoothSize(lgfx::LGFXBase& gfx, float size) {
  gfx.setTextSize(size);
}

void displayFontSetBitmap(lgfx::LGFXBase& gfx, const lgfx::GFXfont* font) {
  gfx.setFont(font);
  gfx.setTextSize(1);
}
