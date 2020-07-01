#ifndef __wipeanimation__h
#define __wipeanimation__h
#include "ColorCyclingAnimation.h"
#include "../FrameBuffer.h"

typedef enum
{
    LEFT,
    RIGHT,
    UP,
    DOWN
} WipeDirectionEnum;

class Wipe : public ColorCyclingAnimation
{
protected:
    WipeDirectionEnum direction;

public:
    Wipe(WipeDirectionEnum direction)
    {
        this->direction = direction;
    }
    void draw(FrameBuffer *frame) override
    {
        int height = frame->height * elapsed / 1000;
        int width = frame->width * elapsed / 1000;
        switch (direction)
        {
        case UP:
        {
            frame->fillRect(0, frame->height - height, frame->width, height, colors[colorIndex]);
            break;
        }
        case DOWN:
        {
            frame->fillRect(0, 0, frame->width, height, colors[colorIndex]);
            break;
        }
        case LEFT:
        {
            frame->fillRect(0, 0, width, frame->height, colors[colorIndex]);
            break;
        }
        case RIGHT:
        {
            frame->fillRect(frame->width - width, 0, width, frame->height, colors[colorIndex]);
            break;
        }
        }
    }
};

#endif