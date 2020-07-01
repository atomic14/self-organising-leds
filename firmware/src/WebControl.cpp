#include <Arduino.h>
#include <FS.h>
#include "SPIFFS.h"
#include "Vision.h"
#include "WebControl.h"
#include "PapertrailLogger.h"
#include "ESPAsyncWebServer.h"
#include "AsyncJson.h"
#include "Leds.h"
#include "FrameBuffer.h"
#include "Point2D.h"

AsyncWebServer server(80);

WebControl::WebControl(PapertrailLogger *logger, Vision *vision, Leds *leds, FrameBuffer *frameBuffer)
{
    currentMode = RUNNING;
    this->logger = logger;
    this->vision = vision;
    this->leds = leds;
    this->frameBuffer = frameBuffer;

    SPIFFS.begin();

    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "GET, PUT, POST");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "*");

    server.on("/raw_image", [this](AsyncWebServerRequest *request) {
        digitalWrite(33, HIGH);
        Frame *frame = this->vision->getFrame();
        AsyncResponseStream *response = request->beginResponseStream("application/octet-stream", frame->length);
        response->write(frame->pixels, frame->length);
        request->send(response);
        delete frame;
        digitalWrite(33, LOW);
    });

    server.on("/frame_bffer", [this](AsyncWebServerRequest *request) {
        digitalWrite(33, HIGH);
        int length = this->frameBuffer->width * this->frameBuffer->height * 2;
        AsyncResponseStream *response = request->beginResponseStream("application/octet-stream", length);
        response->write((uint8_t *)this->frameBuffer->pixels, length);
        request->send(response);
        digitalWrite(33, LOW);
    });

    server.on("/led_positions", [this](AsyncWebServerRequest *request) {
        digitalWrite(33, HIGH);
        int length = this->leds->ledCount * sizeof(Point2D);
        AsyncResponseStream *response = request->beginResponseStream("application/octet-stream", length);
        response->write((uint8_t *)this->leds->ledPositions, length);
        request->send(response);
        digitalWrite(33, LOW);
    });

    server.on("/calibrate", [this](AsyncWebServerRequest *request) {
        digitalWrite(33, HIGH);
        request->send(200, "application/json", "OK");
        this->currentMode = CALIBRATING;
        digitalWrite(33, LOW);
    });

    server.on("/pause", [this](AsyncWebServerRequest *request) {
        digitalWrite(33, HIGH);
        request->send(200, "application/json", "OK");
        this->currentMode = PAUSED;
        digitalWrite(33, LOW);
    });

    server.on("/run", [this](AsyncWebServerRequest *request) {
        digitalWrite(33, HIGH);
        request->send(200, "application/json", "OK");
        this->currentMode = RUNNING;
        digitalWrite(33, LOW);
    });

    server.addHandler(new AsyncCallbackJsonWebHandler("/led", [this](AsyncWebServerRequest *request, JsonVariant &json) {
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
        [this](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
            digitalWrite(33, HIGH);
            for (int i = 0; i < this->leds->ledCount; i++)
            {
                this->leds->setLedRGB(i, data[i * 3], data[i * 3 + 1], data[i * 3 + 2]);
            }
            this->leds->show();
            request->send(200, "application/json", "OK");
            digitalWrite(33, LOW);
        });

    server.on("/clear", [this](AsyncWebServerRequest *request) {
        digitalWrite(33, HIGH);
        this->leds->clear();
        request->send(200, "application/json", "OK");
        digitalWrite(33, LOW);
    });

    server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");

    server.onNotFound([](AsyncWebServerRequest *request) {
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
