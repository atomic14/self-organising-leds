#ifndef __animation_base__h
#define __animation_base__h
#include <Arduino.h>

class FrameBuffer;

class AnimationBase
{
protected:
    int elapsed;

public:
    AnimationBase()
    {
        elapsed = 0;
    }
    virtual void draw(FrameBuffer *frame) = 0;
    virtual void reset()
    {
        elapsed = 0;
    }
    virtual void step(int ms)
    {
        elapsed += ms;
    }
    virtual bool isDone() = 0;
};
#endif