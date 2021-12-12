#include "Arduino.h"
#include "AudioAnimation.h"
#include "config.h"
#include "I2SMEMSSampler.h"
#include "ADCSampler.h"
#include "I2SSampler.h"
#include "Palette.h"
#include "../FrameBuffer.h"

void audio_task(void *param)
{
  AudioAnimation *audio = (AudioAnimation *)param;
#ifdef USE_I2S_MIC_INPUT
  I2SSampler *input = new I2SMEMSSampler(I2S_NUM_0, i2s_mic_pins, i2s_mic_Config);
#else
  I2SSampler *input = new ADCSampler(ADC_UNIT_1, ADC1_CHANNEL_7, i2s_adc_config);
#endif
  input->start();
  int16_t *samples = (int16_t *)malloc(sizeof(int16_t) * 1024);

  while (true)
  {
    input->read(samples, 512);
    // calculate the max amplitude
    float max = 0;
    for (int i = 0; i < 512; i++)
    {
      float abs_value = fabs(samples[i]);
      if (abs_value > max)
      {
        max = abs_value;
      }
    }
    // update our running average
    audio->update(max / 32768.0f);
  }
}

AudioAnimation::AudioAnimation()
{
  palette = new Palette();
}

void AudioAnimation::begin()
{
#ifdef USE_I2S_MIC_INPUT
  // turn on the I2S microphone
  pinMode(I2S_MIC_GND, OUTPUT);
  pinMode(I2S_MIC_VCC, OUTPUT);
  digitalWrite(I2S_MIC_GND, LOW);
  digitalWrite(I2S_MIC_VCC, HIGH);
#endif

  // start monitoring the autio input
  xTaskCreate(audio_task, "Audio Task", 4096, this, 0, NULL);
}

void AudioAnimation::update(float new_amplitude)
{
  amplitude = amplitude * 0.5 + new_amplitude * 0.5;
}

void AudioAnimation::render(FrameBuffer *frame, float scale)
{
  frame->clear();
  int color = 255.0f * amplitude * scale;
  if (color > 255)
  {
    colorScale = std::min(2.0f, std::max(colorScale, (float)color / 255.0f));
  }
  else
  {
    if (colorScale > 1.0)
    {
      colorScale = colorScale * 0.95;
    }
  }
  if (colorScale < 1.0)
  {
    colorScale = 1.0;
  }
  uint32_t paletColor = palette->get_color(color / colorScale);
  // draw the current color
  frame->clear();
  int height = frame->height * amplitude * scale;
  if (height > frame->height)
  {
    heightScale = std::min(2.0f, std::max(heightScale, (float)height / (float)frame->height));
    height = frame->height;
  }
  else
  {
    if (heightScale > 1.0)
    {
      heightScale = heightScale * 0.95;
    }
  }
  if (heightScale < 1.0)
  {
    heightScale = 1.0;
  }

  frame->fillRect(0, std::max(0.0f, frame->height - height / heightScale), frame->width, frame->height, paletColor);
}