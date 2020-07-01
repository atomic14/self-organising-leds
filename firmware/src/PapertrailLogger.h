#ifndef __papertrail_logger_h__
#define __papertrail_logger_h__
#include <Arduino.h>

class WiFiUDP;

enum LogLevel
{
    Error = 3,
    Warning = 4,
    Notice = 5,
    Info = 6,
    Debug = 7
};

#define BUFFER_SIZE 200

class PapertrailLogger : public Print
{
private:
    String mHost;
    int mPort;
    WiFiUDP *mWifiUdp;
    LogLevel mLevel;
    String mSystem;
    String mContext;
    String mColor;

    uint8_t mBuffer[BUFFER_SIZE];
    int mBufferPos;

public:
    PapertrailLogger(String host, int port, LogLevel level, String color, String system, String context);
    size_t write(uint8_t c);
    PapertrailLogger *error();
    PapertrailLogger *warning();
    PapertrailLogger *notice();
    PapertrailLogger *debug();
    PapertrailLogger *info();
};

#endif