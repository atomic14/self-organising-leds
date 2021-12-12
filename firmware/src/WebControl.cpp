#include <Arduino.h>
#include <FS.h>
#include "SPIFFS.h"
#include "Vision.h"
#include "WebControl.h"
#include <ESPAsyncWebServer.h>
#include <AsyncJson.h>
#include <ArduinoJson.h>
#include "Leds.h"
#include "FrameBuffer.h"
#include "Point2D.h"

AsyncWebServer server(80);

WebControl::WebControl(Vision *vision, Leds *leds, FrameBuffer *frameBuffer)
{
    currentMode = RUNNING_ANIMATION;
    this->vision = vision;
    this->leds = leds;
    this->frameBuffer = frameBuffer;

    SPIFFS.begin();

    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "GET, PUT, POST");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "*");

    server.on("/rssi", [this](AsyncWebServerRequest *request)
              {
                  long rssi = WiFi.RSSI();
                  char buffer[100];
                  sprintf(buffer, "%ld", rssi);
                  request->send(200, "text/plain", buffer);
              });

    server.on("/raw_image", [this](AsyncWebServerRequest *request)
              {
                  digitalWrite(33, HIGH);
                  Frame *frame = this->vision->getFrame();
                  AsyncResponseStream *response = request->beginResponseStream("application/octet-stream", frame->length);
                  response->write(frame->pixels, frame->length);
                  request->send(response);
                  delete frame;
                  digitalWrite(33, LOW);
              });

    server.on("/frame_bffer", [this](AsyncWebServerRequest *request)
              {
                  digitalWrite(33, HIGH);
                  int length = this->frameBuffer->width * this->frameBuffer->height * 2;
                  AsyncResponseStream *response = request->beginResponseStream("application/octet-stream", length);
                  response->write((uint8_t *)this->frameBuffer->pixels, length);
                  request->send(response);
                  digitalWrite(33, LOW);
              });

    server.on("/led_positions", [this](AsyncWebServerRequest *request)
              {
                  digitalWrite(33, HIGH);
                  int length = this->leds->ledCount * sizeof(Point2D);
                  AsyncResponseStream *response = request->beginResponseStream("application/octet-stream", length);
                  response->write((uint8_t *)this->leds->ledPositions, length);
                  request->send(response);
                  digitalWrite(33, LOW);
              });

    server.on("/calibrate", [this](AsyncWebServerRequest *request)
              {
                  digitalWrite(33, HIGH);
                  request->send(200, "application/json", "OK");
                  this->currentMode = CALIBRATING;
                  digitalWrite(33, LOW);
              });

    server.on("/pause", [this](AsyncWebServerRequest *request)
              {
                  digitalWrite(33, HIGH);
                  request->send(200, "application/json", "OK");
                  this->currentMode = PAUSED;
                  digitalWrite(33, LOW);
                  Serial.println("Pause");
              });

    server.on("/run_animation", [this](AsyncWebServerRequest *request)
              {
                  digitalWrite(33, HIGH);
                  request->send(200, "application/json", "OK");
                  this->currentMode = RUNNING_ANIMATION;
                  digitalWrite(33, LOW);
                  Serial.println("Running Animations");
              });

    server.on("/run_audio", [this](AsyncWebServerRequest *request)
              {
                  digitalWrite(33, HIGH);
                  request->send(200, "application/json", "OK");
                  this->currentMode = RUNNING_AUDIO;
                  digitalWrite(33, LOW);
                  Serial.println("Running Audio");
              });

    server.on("/run_flames", [this](AsyncWebServerRequest *request)
              {
                  digitalWrite(33, HIGH);
                  request->send(200, "application/json", "OK");
                  this->currentMode = RUNNING_FLAMES;
                  digitalWrite(33, LOW);
                  Serial.println("Running Flames");
              });

    server.on("/run_random", [this](AsyncWebServerRequest *request)
              {
                  digitalWrite(33, HIGH);
                  request->send(200, "application/json", "OK");
                  this->currentMode = RUNNING_RANDOM;
                  digitalWrite(33, LOW);
                  Serial.println("Running Random");
              });

    server.addHandler(new AsyncCallbackJsonWebHandler("/led", [this](AsyncWebServerRequest *request, JsonVariant &json)
                                                      {
                                                          digitalWrite(33, HIGH);
                                                          const JsonObject &jsonObj = json.as<JsonObject>();
                                                          this->leds->setLedRGB(jsonObj["l"], jsonObj["r"], jsonObj["g"], jsonObj["b"]);
                                                          request->send(200, "application/json", "OK");
                                                          digitalWrite(33, LOW);
                                                      }));

    server.on(
        "/leds", WebRequestMethod::HTTP_POST,
        // nothing to do in the request
        [](AsyncWebServerRequest *request) {},
        // no file upload
        NULL,
        // handle the body - we should really buffer up the body data but currently it's short enough...
        [this](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
        {
            digitalWrite(33, HIGH);
            // store the data in the request tempObject
            if (!request->_tempObject)
            {
                request->_tempObject = malloc(total);
            }
            memcpy(((uint8_t *)request->_tempObject) + index, data, len);
            if (index + len == total)
            {
                uint8_t *rgbData = (uint8_t *)request->_tempObject;
                for (int i = 0; i < this->leds->ledCount; i++)
                {
                    this->leds->setLedRGB(i, rgbData[i * 3], rgbData[i * 3 + 1], rgbData[i * 3 + 2], false);
                }
                this->leds->show();
                request->send(200, "application/json", "OK");
            }
            digitalWrite(33, LOW);
        });

    server.on(
        "/scale", WebRequestMethod::HTTP_POST,
        // nothing to do in the rquest
        [](AsyncWebServerRequest *request) {},
        // no file to upload
        NULL,
        // handle the body - we should really buffer up the body data but currently it's short enough...
        [this](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
        {
            // parse the data as json
            DynamicJsonDocument doc(1024);
            deserializeJson(doc, data, len);
            this->audioScale = doc["scale"];
            request->send(200, "OK");
            Serial.println("Audio scale set to " + String(this->audioScale));
        });

    server.on(
        "/positions", WebRequestMethod::HTTP_POST,
        // nothing to do in the request
        [](AsyncWebServerRequest *request) {},
        // no file upload
        NULL,
        // handle the body - we should really buffer up the body data but currently it's short enough...
        [this](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
        {
            digitalWrite(33, HIGH);
            // store the data in the request tempObject
            if (!request->_tempObject)
            {
                request->_tempObject = malloc(total);
            }
            memcpy(((uint8_t *)request->_tempObject) + index, data, len);
            if (index + len == total)
            {
                uint16_t *positions = (uint16_t *)request->_tempObject;
                int minx = 1000;
                int miny = 1000;
                int maxx = 0;
                int maxy = 0;
                for (int i = 0; i < this->leds->ledCount; i++)
                {
                    int x = positions[i * 2];
                    int y = positions[i * 2 + 1];
                    Serial.printf("Led %d is at %d %d\n", i, x, y);
                    minx = min(x, minx);
                    miny = min(y, miny);
                    maxx = max(x, maxx);
                    maxy = max(y, maxy);
                    this->leds->setLedPosition(i, x, y);
                }
                this->leds->setBounds(minx, miny, maxx, maxy);
                this->leds->storePositions();
                this->leds->isCalibrated = true;
                Serial.printf("Bounds %d, %d, %d, %d\n", minx, miny, maxx, maxy);
                request->send(200, "application/json", "OK");
            }
            digitalWrite(33, LOW);
        });

    server.on("/clear", [this](AsyncWebServerRequest *request)
              {
                  digitalWrite(33, HIGH);
                  this->leds->clear();
                  request->send(200, "application/json", "OK");
                  digitalWrite(33, LOW);
              });

    server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");

    server.onNotFound([](AsyncWebServerRequest *request)
                      {
                          Serial.println("NOT FOUND: " + request->url());
                          digitalWrite(33, HIGH);
                          if (request->method() == HTTP_OPTIONS)
                          {
                              request->send(200);
                          }
                          else
                          {
                              request->send(404);
                          }
                          digitalWrite(33, LOW);
                      });
    server.begin();
}
