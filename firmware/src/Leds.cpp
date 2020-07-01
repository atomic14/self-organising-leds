#include <Arduino.h>
#include <FastLED.h>
#include "Leds.h"
#include "Point2D.h"
#include "PapertrailLogger.h"
#include "Vision.h"
#include "OTA.h"
#include "FrameBuffer.h"
#define LED_PIN 16

Leds::Leds(int ledCount, PapertrailLogger *logger, OTA *ota)
{
    this->logger = logger;
    this->ota = ota;
    // allocate leds
    this->ledCount = ledCount;
    leds = new CRGB[ledCount];
    FastLED.addLeds<WS2811, LED_PIN, RGB>(leds, ledCount);
    // allocate space to store the positions of the Leds
    ledPositions = new Point2D[ledCount];
    for (int ledIdx = 0; ledIdx < ledCount; ledIdx++)
    {
        ledPositions[ledIdx] = Point2D({-1, -1});
    }
}

Leds::~Leds()
{
    delete[] leds;
    delete[] ledPositions;
}

bool Leds::calibrate(Vision *vision)
{
    logger->info()->println("Started calibration");
    minX = 10000;
    maxX = -1;
    minY = 10000;
    maxY = -1;
    // set all the LEDs to off and then turn them on one at a time and detect the difference between the original and the new frame
    clear();
    delay(30);
    for (int ledIdx = 0; ledIdx < ledCount; ledIdx++)
    {
        Frame *allOffFrame = vision->getFrame();
        if (!allOffFrame)
        {
            // failed to get a frame, give up
            return false;
        }
        setLedRGB(ledIdx, 255, 255, 255, true);
        int pointWeight = 0;
        int numPoints = 0;
        int xPos = 0;
        int yPos = 0;
        int maxDifference = 0;
        Frame *oneOnFrame = vision->getFrame();
        if (!oneOnFrame)
        {
            // failed to get a frame, give up
            delete allOffFrame;
            return false;
        }
        uint8_t *oneOnPixels = oneOnFrame->pixels;
        uint8_t *allOffPixels = allOffFrame->pixels;
        for (int y = 0; y < allOffFrame->height; y++)
        {
            for (int x = 0; x < allOffFrame->width; x++)
            {
                int delta = (int)(*oneOnPixels) - (int)(*allOffPixels);
                if (delta > maxDifference)
                {
                    maxDifference = delta;
                }
                allOffPixels++;
                oneOnPixels++;
            }
        }
        oneOnPixels = oneOnFrame->pixels;
        allOffPixels = allOffFrame->pixels;
        for (int y = 0; y < allOffFrame->height; y++)
        {
            for (int x = 0; x < allOffFrame->width; x++)
            {
                int delta = (int)(*oneOnPixels) - (int)(*allOffPixels);
                if (delta > (maxDifference >> 1))
                {
                    numPoints++;
                    pointWeight += delta;
                    xPos += x * delta;
                    yPos += y * delta;
                }
                allOffPixels++;
                oneOnPixels++;
            }
        }
        delete oneOnFrame;
        if (pointWeight > 0)
        {
            int x = xPos / pointWeight;
            int y = yPos / pointWeight;
            minX = min(minX, x);
            minY = min(minY, y);
            maxX = max(maxX, x);
            maxY = max(maxY, y);
            ledPositions[ledIdx] = Point2D({x, y});
        }
        else
        {
            logger->info()->printf("Failed to get location for LED %d\n", ledIdx);
            ledPositions[ledIdx] = Point2D({-1, -1});
        }
        // set the pixel back to off
        setLedRGB(ledIdx, 0, 0, 0, true);
        delete allOffFrame;
    }
    clear();
    show();
    digitalWrite(33, LOW);
    logger->info()->println("Done calibration");
    return true;
}

void Leds::clear()
{
    memset(leds, 0, sizeof(CRGB) * ledCount);
    FastLED.show();
}

void Leds::setLedRGB(int led, uint8_t r, uint8_t g, uint8_t b, bool show)
{
    leds[led] = CRGB(r, g, b);
    if (show)
    {
        FastLED.show();
    }
}

void Leds::show()
{
    FastLED.show();
}

void Leds::showImage(FrameBuffer *frameBuffer)
{
    float scaleX = (float)(frameBuffer->width - 1) / (maxX - minX);
    float scaleY = (float)(frameBuffer->height - 1) / (maxY - minY);
    for (int ledIdx = 0; ledIdx < ledCount; ledIdx++)
    {
        // do we actually know where the LED is?
        if (ledPositions[ledIdx].x == -1)
        {
            setLedRGB(ledIdx, 20, 20, 20);
            continue;
        }
        // the position of the LED in the image
        float x = (ledPositions[ledIdx].x - minX) * scaleX;
        float y = (ledPositions[ledIdx].y - minY) * scaleY;
        // get the pixel value from the framebuffer
        RGBPixel rgb = frameBuffer->getRGBColorAtPoint(x, y);
        setLedRGB(ledIdx, rgb.r, rgb.g, rgb.b, false);
    }
    show();
}