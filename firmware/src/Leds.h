#ifndef __leds__h
#define __leds__h

#include <Arduino.h>
#include "FastLED.h"
#include "RGBPixel.h"

class Point2D;
class Vision;
class OTA;
class FrameBuffer;
class Preferences;

class Leds
{
private:
    Preferences *preferences;
    OTA *ota;
    CRGB *leds;
    void setLEDColor(int position, const RGBPixel &color);
    int16_t minX, maxX, minY, maxY;

public:
    int ledCount;
    bool isCalibrated;
    Point2D *ledPositions;
    Leds(int ledCount, OTA *ota);
    ~Leds();
    bool calibrate(Vision *vision);
    void showImage(FrameBuffer *frameBuffer);
    void setLedRGB(int led, uint8_t r, uint8_t g, uint8_t b, bool show = true);
    void setLedPosition(int led, uint16_t x, uint16_t y);
    void storePositions();
    void setBounds(int16_t minx, int16_t miny, int16_t maxx, int16_t maxy);
    void clear();
    void show();
};

#endif