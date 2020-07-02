#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include "OTA.h"

OTA::OTA(const char *hostname)
{
    ArduinoOTA.setHostname(hostname);
    ArduinoOTA
        .onStart([this]() {
            String type;
            if (ArduinoOTA.getCommand() == U_FLASH)
                type = "sketch";
            else // U_SPIFFS
                type = "filesystem";

            // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
            Serial.println("Start updating " + type);
        })
        .onEnd([this]() {
            Serial.println("Update Complete");
            digitalWrite(33, LOW);
        })
        .onProgress([](unsigned int progress, unsigned int total) {
            unsigned int percent = progress / (total / 100);
            digitalWrite(33, (percent % 2) == 1 ? HIGH : LOW);
            // Serial.printf("Progress: %u%%\r", percent);
        })
        .onError([this](ota_error_t error) {
            Serial.printf("Error[%u]: ", error);
            if (error == OTA_AUTH_ERROR)
                Serial.println("Auth Failed");
            else if (error == OTA_BEGIN_ERROR)
                Serial.println("Begin Failed");
            else if (error == OTA_CONNECT_ERROR)
                Serial.println("Connect Failed");
            else if (error == OTA_RECEIVE_ERROR)
                Serial.println("Receive Failed");
            else if (error == OTA_END_ERROR)
                Serial.println("End Failed");
            digitalWrite(33, LOW);
        });
    ArduinoOTA.begin();
}

void OTA::loop()
{
    ArduinoOTA.handle();
}