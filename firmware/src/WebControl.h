#ifndef __webcontrol__h
#define __webcontrol__h

class Leds;
class PapertrailLogger;
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
    PapertrailLogger *logger;
    Vision *vision;
    FrameBuffer *frameBuffer;
    Leds *leds;

public:
    MODE currentMode;
    WebControl(PapertrailLogger *logger, Vision *vision, Leds *leds, FrameBuffer *frameBuffer);
};

#endif