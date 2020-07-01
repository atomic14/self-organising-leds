#include "FrameBuffer.h"
#include <Fonts/FreeMonoBold18pt7b.h>

FrameBuffer::FrameBuffer(int16_t width, int16_t height) : Adafruit_GFX(width, height)
{
    this->width = width;
    this->height = height;
    this->setFont(&FreeMonoBold18pt7b);
    pixels = (uint16_t *)malloc(2 * width * height);
    memset(pixels, 0, 2 * width * height);
    rows = (uint16_t **)malloc(sizeof(uint16_t *) * height);
    for (int y = 0; y < height; y++)
    {
        rows[y] = pixels + y * width;
    }
}
FrameBuffer::~FrameBuffer()
{
    free(pixels);
    free(rows);
}

void FrameBuffer::drawPixel(int16_t x, int16_t y, uint16_t color)
{
    if (x >= 0 && y >= 0 && x < width && y < height)
    {
        rows[y][x] = color;
    }
}

void FrameBuffer::writePixel(int16_t x, int16_t y, uint16_t color)
{
    if (x >= 0 && y >= 0 && x < width && y < height)
    {
        rows[y][x] = color;
    }
}
void FrameBuffer::writeFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
{
    if (x < 0 || x >= width)
        return;
    int16_t y2 = min(int16_t(height - 1), (int16_t)(y + h));
    for (int i = y; i < y2; i++)
    {
        if (i > 0)
        {
            rows[i][x] = color;
        }
    }
}
void FrameBuffer::writeFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
{
    if (y < 0 || y >= height)
        return;
    int16_t x2 = min(int16_t(width - 1), (int16_t)(x + w));
    for (int i = x; i < x2; i++)
    {
        if (i > 0)
        {
            rows[y][i] = color;
        }
    }
}

void FrameBuffer::fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
                           uint16_t color)
{
    for (int i = y; i < y + h; i++)
    {
        writeFastHLine(x, i, w, color);
    }
}

void FrameBuffer::clear()
{
    memset(pixels, 0, 2 * width * height);
}

RGBPixel FrameBuffer::getRGBColorAtPoint(float x, float y)
{
    if (x < 0 || y < 0 || x >= width || y >= height)
    {
        return {0, 0, 0};
    }
    // bilinear interpolation - https://en.wikipedia.org/wiki/Bilinear_interpolation
    // top left pixel location
    int x1 = int(x);
    int y1 = int(y);
    // bottom right pixel location
    int x2 = min(width - 1, int(x + 1));
    int y2 = min(height - 1, int(y + 1));

    RGBPixel q11 = getRGBColorAtPoint(x1, y1);
    RGBPixel q21 = getRGBColorAtPoint(x2, y1);
    float xy1_r = (x2 - x) * q11.r + (x - x1) * q21.r;
    float xy1_g = (x2 - x) * q11.g + (x - x1) * q21.g;
    float xy1_b = (x2 - x) * q11.b + (x - x1) * q21.b;

    RGBPixel q12 = getRGBColorAtPoint(x1, y2);
    RGBPixel q22 = getRGBColorAtPoint(x2, y2);
    float xy2_r = (x2 - x) * q12.r + (x - x1) * q22.r;
    float xy2_g = (x2 - x) * q12.g + (x - x1) * q22.g;
    float xy2_b = (x2 - x) * q12.b + (x - x1) * q22.b;

    float r = (y2 - y) * xy1_r + (y - y1) * xy2_r;
    float g = (y2 - y) * xy1_g + (y - y1) * xy2_g;
    float b = (y2 - y) * xy1_b + (y - y1) * xy2_b;
    return {r, g, b};
}
