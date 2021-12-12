#ifndef __webcontrol__h
#define __webcontrol__h

class Leds;
class Vision;
class FrameBuffer;

typedef enum
{
    RUNNING_ANIMATION,
    RUNNING_AUDIO,
    RUNNING_FLAMES,
    RUNNING_RANDOM,
    CALIBRATING,
    PAUSED
} MODE;

class WebControl
{
private:
    Vision *vision;
    FrameBuffer *frameBuffer;
    Leds *leds;

public:
    volatile MODE currentMode;
    float audioScale = 1.0;
    WebControl(Vision *vision, Leds *leds, FrameBuffer *frameBuffer);
};

#endif