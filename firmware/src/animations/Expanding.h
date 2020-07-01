#ifndef __expandinganimation__h
#define __expandinganimation__h

#include "ColorCyclingAnimation.h"
#include "../FrameBuffer.h"

class Expanding : public ColorCyclingAnimation
{
public:
    void draw(FrameBuffer *frame) override
    {
        int width = frame->width * elapsed / 1000;
        int height = frame->height * elapsed / 1000;
        frame->fillRect((frame->width - width) / 2, (frame->height - height) / 2, width, height, colors[colorIndex]);
    }
};

#endif