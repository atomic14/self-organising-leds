#ifndef __leds__h
#define __leds__h

#include <Arduino.h>
#include "FastLED.h"
#include "RGBPixel.h"

class Point2D;
class Vision;
class OTA;
class FrameBuffer;

class Leds
{
private:
    OTA *ota;
    CRGB *leds;
    void setLEDColor(int position, const RGBPixel &color);
    int minX, maxX, minY, maxY;

public:
    int ledCount;
    Point2D *ledPositions;
    Leds(int ledCount, OTA *ota);
    ~Leds();
    bool calibrate(Vision *vision);
    void showImage(FrameBuffer *frameBuffer);
    void setLedRGB(int led, uint8_t r, uint8_t g, uint8_t b, bool show = true);
    void clear();
    void show();
};

#endif