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
};

class Vision
{

public:
    Vision();
    Frame *getFrame();
};

#endif