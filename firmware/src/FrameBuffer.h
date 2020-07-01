#ifndef __framebuffer__h
#define __framebuffer__h
#include <Adafruit_GFX.h>
#include "RGBPixel.h"

class FrameBuffer : public Adafruit_GFX
{
public:
    uint16_t *pixels;
    uint16_t **rows;
    int16_t width, height;
    FrameBuffer(int16_t width, int16_t height);
    ~FrameBuffer();
    void drawPixel(int16_t x, int16_t y, uint16_t color) override;
    void writePixel(int16_t x, int16_t y, uint16_t color) override;
    void writeFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) override;
    void writeFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) override;
    void fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
                  uint16_t color) override;
    void clear();
    // return the pixel at x and y as RGB
    RGBPixel getRGBColorAtPoint(int x, int y)
    {
        uint16_t color = rows[y][x];
        uint8_t r = ((color & 0xF800) >> 11) * 255 / 31;
        uint8_t g = ((color & 0x07E0) >> 5) * 255 / 63;
        uint8_t b = (color & 0x001F) * 255 / 31;
        return {r, g, b};
    }
    // return the bilinear interpolated pixel for x and y
    RGBPixel getRGBColorAtPoint(float x, float y);
};

#endif
