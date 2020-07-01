#ifndef __colorcyclinganimation_base__h
#define __colorcyclinganimation_base__h
#include "AnimationBase.h"
#include "../FrameBuffer.h"

class ColorCyclingAnimation : public AnimationBase
{
protected:
    int colorIndex;
    uint16_t colors[8] = {
        0x001F,
        0xF800,
        0x07E0,
        0x07FF,
        0xF81F,
        0xFFE0,
        0xFFFF,
        0x0000};

public:
    ColorCyclingAnimation() : AnimationBase()
    {
        colorIndex = 0;
    }
    void reset() override
    {
        AnimationBase::reset();
        colorIndex = 0;
    }
    void step(int ms) override
    {
        AnimationBase::step(ms);
        if (elapsed > 1000)
        {
            elapsed = 0;
            colorIndex++;
        }
    }
    bool isDone() override
    {
        return colorIndex == 8;
    }
};

#endif