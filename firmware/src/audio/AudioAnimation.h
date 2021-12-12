#pragma once

class Palette;
class FrameBuffer;
class Processor;

class AudioAnimation
{
private:
  float amplitude;
  Palette *palette;
  float colorScale = 1.0;
  float heightScale = 1.0;

public:
  AudioAnimation();
  void begin();
  float get_amplitude()
  {
    return amplitude;
  }
  void update(float new_amplitude);
  void render(FrameBuffer *frame, float scale);
};