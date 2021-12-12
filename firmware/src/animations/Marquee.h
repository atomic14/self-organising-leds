#ifndef __marquee__h
#define __marquee__h

#include "AnimationBase.h"
#include "FrameBuffer.h"

class Marquee : public AnimationBase
{
private:
    String message;
    int msgLen;
    int totalTime;

public:
    Marquee(const String &message)
    {
        this->message = message;
        this->msgLen = message.length();
        totalTime = this->msgLen * 1000;
    }
    void draw(FrameBuffer *frame) override
    {
        int offset = (msgLen + 2) * frame->width * elapsed / totalTime;
        frame->clear();
        for (int i = 0; i < message.length(); i++)
        {
            frame->drawChar(i * 100 - offset + 200, 80, message.charAt(i), 0x001F, 0, 3);
        }
    }
    bool isDone() override
    {
        return elapsed > totalTime;
    }
};

#endif