#include "Palette.h"
#include <FastLED.h>

uint16_t rgb_to_uint16(float r, float g, float b)
{
  int ri = (int)(r * 31.0f);
  int gi = (int)(g * 63.0f);
  int bi = (int)(b * 31.0f);
  // Serial.printf("%d %d %d\n", ri, gi, bi);
  return (ri << 11) | (gi << 5) | bi;
}

DEFINE_GRADIENT_PALETTE(blackToRed){
    0, 64, 0, 128,   //dark mauve
    128, 255, 0, 255,   //green
    164, 255, 220, 0, //bright yellow
    192, 255, 128, 0, //bright orange
    255, 255, 0, 0    //full red
};
CRGBPalette256 blackToRedPal = blackToRed;

Palette::Palette()
{
  for (int i = 0; i < 256; ++i)
  {
    CRGB color = ColorFromPalette(blackToRedPal, i, 255, LINEARBLEND);
    colors[i] = rgb_to_uint16(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f);
  }
}
