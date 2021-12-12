#include <FastLED.h>

#define PALETTE_SIZE 256

class FrameBuffer;
class Flames
{
    float **cells1;
    float **cells2;

    CRGB palette[PALETTE_SIZE];

public:
    Flames();
    void update();
    void render(FrameBuffer *fb);
    void setHueRange(int start, int end);
};