#include "flames.h"
#include "../FrameBuffer.h"

#define BRIGHTNESS 255

#define WIDTH 20
#define HEIGHT 17

#define TEMP_INCREASE 120
#define MIN_TEMP_INCREASE 0
#define TEMP_LOSS 0.9

Flames::Flames()
{
    cells1 = (float **)malloc(sizeof(float *) * HEIGHT);
    cells2 = (float **)malloc(sizeof(float *) * HEIGHT);
    for (int y = 0; y < HEIGHT; y++)
    {
        cells1[y] = (float *)malloc(sizeof(float) * WIDTH);
        cells2[y] = (float *)malloc(sizeof(float) * WIDTH);
        memset(cells1[y], 0, sizeof(float) * WIDTH);
        memset(cells2[y], 0, sizeof(float) * WIDTH);
    }
    // initialise the palette
    setHueRange(0, 60);
}

void Flames::setHueRange(int start, int end)
{
    for (int x = 0; x < PALETTE_SIZE; x++)
    {
        float proportion = float(x) / (PALETTE_SIZE - 1.0f);
        float angle = start + (end - start) * proportion;
        // no point trying to turn the LED completely off as they don't fade out nicely and we can't dither
        float brightness = 1;
        if (x < PALETTE_SIZE / 2)
        {
            brightness = 1.0f * proportion * 2;
        }
        float saturation = 1 - 0.25 * proportion;
        palette[x] = CRGB(CHSV(255.0f * angle / 360.0f, 255.0f * saturation, 255 * brightness));
    }
}

void Flames::update()
{
    // add fuel to the fire
    for (int x = 0; x < WIDTH; x++)
    {
        cells1[0][x] += std::max(MIN_TEMP_INCREASE, (int)random(TEMP_INCREASE));
    }
    // clear the next state
    for (int y = 0; y < HEIGHT; y++)
    {
        memset(cells2[y], 0, sizeof(float) * WIDTH);
    }
    // burn baby
    for (int y = 0; y < HEIGHT - 1; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            cells1[y][x] *= TEMP_LOSS;
            cells2[y][x] += 0.2 * cells1[y][x];
            cells2[y + 1][x] += 0.6 * cells1[y][x];
            cells2[y + 1][(x + 1) % WIDTH] += 0.1 * cells1[y][x];
            cells2[y + 1][(x - 1 + WIDTH) % WIDTH] += 0.1 * cells1[y][x];
        }
    }
    // swap buffers
    float **tmp = cells1;
    cells1 = cells2;
    cells2 = tmp;
}

void Flames::render(FrameBuffer *fb)
{
    for (int x = 0; x < fb->width; x++)
    {
        for (int y = 0; y < fb->height; y++)
        {
            int fy = y * HEIGHT / fb->height;
            int fx = x * WIDTH / fb->width;
            // handle our missing LED
            float temp = cells1[fy][fx] / 90.0f;
            int color = (PALETTE_SIZE - 1) * std::max(0.0f, std::min(1.0f, temp));
            CRGB &c = palette[color];
            // convert to 16 bit color
            uint16_t c16 = (c.r >> 3) << 11 | (c.g >> 2) << 5 | (c.b >> 3);
            fb->drawPixel(x, fb->height - y - 1, c16);
        }
    }
}