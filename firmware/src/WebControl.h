#ifndef __webcontrol__h
#define __webcontrol__h

class Leds;
class Vision;
class FrameBuffer;

typedef enum
{
    RUNNING,
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
    MODE currentMode;
    WebControl(Vision *vision, Leds *leds, FrameBuffer *frameBuffer);
};

#endif