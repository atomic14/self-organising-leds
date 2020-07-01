#include <Arduino.h>
#include <WiFi.h>
#include "vision.h"
#include "PapertrailLogger.h"
#include "ota.h"
#include "webcontrol.h"
#include "leds.h"
#include "FrameBuffer.h"
#include <Fonts/FreeSans24pt7b.h>
#include "animations/Expanding.h"
#include "animations/Wipe.h"
#include "animations/Marquee.h"

const char *ssid = << YOUR SSID >> ;
const char *password = << YOU PASSWORD >> ;

#define LED_COUNT 100

Vision *vision;
OTA *ota = NULL;
WebControl *webControl = NULL;
PapertrailLogger *logger = NULL;
Leds *leds = NULL;
FrameBuffer *frameBuffer;

void setup()
{
  Serial.begin(115200);
  Serial.begin(115200);
  Serial.println("Booting");
  // switch the built in red led to output
  pinMode(33, OUTPUT);
  // switch it on
  digitalWrite(33, LOW);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
  logger = new PapertrailLogger("logs5.papertrailapp.com", 34663, LogLevel::Info, "\033[0;34m", "leds", "ledsdisplay");
  logger->println("Started up");
  ota = new OTA(logger, "espcam");
  leds = new Leds(LED_COUNT, logger, ota);
  vision = new Vision(logger);
  webControl = new WebControl(logger, vision, leds, frameBuffer);
  leds->calibrate(vision);
  frameBuffer = new FrameBuffer(100, 100);
  frameBuffer->clear();
}

AnimationBase *animations[5] = {
    new Expanding(),
    new Wipe(LEFT),
    new Wipe(UP),
    new Wipe(DOWN),
    new Wipe(RIGHT),
    //new Marquee("HELLO WORLD")
};
int animationIndex = 0;
unsigned long lastTime = 0;

void loop()
{
  switch (webControl->currentMode)
  {
  case CALIBRATING:
  {
    leds->calibrate(vision);
    webControl->currentMode = RUNNING;
    lastTime = 0;
    break;
  }
  case PAUSED:
  {
    delay(100);
    lastTime = 0;
    break;
  }
  case RUNNING:
  {
    unsigned long currentTime = millis();
    if (lastTime == 0)
    {
      lastTime = currentTime;
    }
    int elapsed = currentTime - lastTime;
    lastTime = currentTime;
    AnimationBase *currentAnimation = animations[animationIndex];
    currentAnimation->step(elapsed);
    if (currentAnimation->isDone())
    {
      animationIndex = (animationIndex + 1) % 5;
      currentAnimation = animations[animationIndex];
      currentAnimation->reset();
    }
    currentAnimation->draw(frameBuffer);
    leds->showImage(frameBuffer);
  }
  }
  ota->loop();
}
