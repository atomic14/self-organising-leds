#pragma once

#include <freertos/FreeRTOS.h>
#include <driver/i2s.h>

// save to SPIFFS instead of SD Card?
// #define USE_SPIFFS 1

// sample rate for the system
#define SAMPLE_RATE 16000

// are you using an I2S microphone - comment this out if you want to use an analog mic and ADC input
#define USE_I2S_MIC_INPUT
// are you using an I2S amplifier - comment this out if you want to use the built in DAC
#define USE_I2S_SPEAKER_OUTPUT

// I2S Microphone Settings
// Which channel is the I2S microphone on? I2S_CHANNEL_FMT_ONLY_LEFT or I2S_CHANNEL_FMT_ONLY_RIGHT
// Generally they will default to LEFT - but you may need to attach the L/R pin to GND
#define I2S_MIC_CHANNEL I2S_CHANNEL_FMT_ONLY_LEFT
// #define I2S_MIC_CHANNEL I2S_CHANNEL_FMT_ONLY_RIGHT
#define I2S_MIC_SERIAL_CLOCK GPIO_NUM_27
#define I2S_MIC_LEFT_RIGHT_CLOCK GPIO_NUM_14
#define I2S_MIC_SERIAL_DATA GPIO_NUM_15
#define I2S_MIC_GND GPIO_NUM_25
#define I2S_MIC_VCC GPIO_NUM_26
#define I2S_LR GPIO_NUM_4

// Analog Microphone Settings - ADC1_CHANNEL_7 is GPIO35
#define ADC_MIC_CHANNEL ADC1_CHANNEL_7

// speaker settings
#define I2S_SPEAKER_SERIAL_CLOCK GPIO_NUM_19
#define I2S_SPEAKER_LEFT_RIGHT_CLOCK GPIO_NUM_27
#define I2S_SPEAKER_SERIAL_DATA GPIO_NUM_18

// record button
#define GPIO_BUTTON GPIO_NUM_23

// sdcard
#define PIN_NUM_MISO GPIO_NUM_4
#define PIN_NUM_CLK GPIO_NUM_14
#define PIN_NUM_MOSI GPIO_NUM_15
#define PIN_NUM_CS GPIO_NUM_25

// i2s config for using the internal ADC
extern i2s_config_t i2s_adc_config;
// i2s config for reading from of I2S
extern i2s_config_t i2s_mic_Config;
// i2s microphone pins
extern i2s_pin_config_t i2s_mic_pins;
// i2s speaker pins
extern i2s_pin_config_t i2s_speaker_pins;
