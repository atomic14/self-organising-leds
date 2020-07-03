#ifndef __vision__h
#define __vision__h
#include "esp_camera.h"

class Frame
{
public:
    uint8_t *pixels;
    int width;
    int height;
    int length;
    Frame(camera_fb_t *fb);
    ~Frame();
    void guassianBlur()
    {
        float kernel[] = {
            9,
            10,
            10,
            11,
            12,
            13,
            13,
            14,
            14,
            15,
            15,
            14,
            14,
            14,
            13,
            13,
            12,
            11,
            10,
            10,
            9};

        uint8_t *horizontal = (uint8_t *)malloc(width * height);
        for (int y = 0; y < height; y++)
        {
            int row = y * width;
            for (int x = 0; x < width; x++)
            {
                int value = 0;
                int divider = 0;
                for (int k = -10; k <= 10; k++)
                {
                    if (k + x >= 0 && k + x < width)
                    {
                        value += pixels[row + x + k] * kernel[k + 10];
                        divider += kernel[k + 10];
                    }
                }
                horizontal[row + x] = value / divider;
            }
        }
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                int value = 0;
                int divider = 0;
                for (int k = -10; k <= 10; k++)
                {
                    if (k + y >= 0 && k + y < height)
                    {
                        value += horizontal[(k + y) * width + x] * kernel[k + 10];
                        divider += kernel[k + 10];
                    }
                }
                pixels[y * width + x] = value / divider;
            }
        }
        free(horizontal);
    }
};

class Vision
{

public:
    Vision(bool enableCamera);
    Frame *getFrame();
};

#endif