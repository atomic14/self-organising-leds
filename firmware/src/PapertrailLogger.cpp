#include <WiFiUdp.h>
#include "PapertrailLogger.h"

const int FacilityCode = 16;

PapertrailLogger::PapertrailLogger(String host, int port, LogLevel level, String color, String system, String context)
{
    mHost = host;
    mPort = port;
    mLevel = level;
    mColor = color;
    mSystem = system;
    mContext = context;
    mBufferPos = 0;
    mWifiUdp = new WiFiUDP();
}

PapertrailLogger *PapertrailLogger::error()
{
    static PapertrailLogger *logger = NULL;
    if (!logger)
    {
        logger = new PapertrailLogger(mHost, mPort, LogLevel::Error, "\033[0;31m", mSystem, mContext);
    }
    return logger;
}
PapertrailLogger *PapertrailLogger::warning()
{
    static PapertrailLogger *logger = NULL;
    if (!logger)
    {
        logger = new PapertrailLogger(mHost, mPort, LogLevel::Warning, "\033[0;33m", mSystem, mContext);
    }
    return logger;
}
PapertrailLogger *PapertrailLogger::notice()
{
    static PapertrailLogger *logger = NULL;
    if (!logger)
    {
        logger = new PapertrailLogger(mHost, mPort, LogLevel::Notice, "\033[0;36m", mSystem, mContext);
    }
    return logger;
}
PapertrailLogger *PapertrailLogger::debug()
{
    static PapertrailLogger *logger = NULL;
    if (!logger)
    {
        logger = new PapertrailLogger(mHost, mPort, LogLevel::Debug, "\033[0;32m", mSystem, mContext);
    }
    return logger;
}
PapertrailLogger *PapertrailLogger::info()
{
    static PapertrailLogger *logger = NULL;
    if (!logger)
    {
        logger = new PapertrailLogger(mHost, mPort, LogLevel::Info, "\033[0;34m", mSystem, mContext);
    }
    return logger;
}

size_t PapertrailLogger::write(uint8_t c)
{
    // check to see if we've reached the end of our buffer or received a newline character
    if (c == '\n' || mBufferPos == BUFFER_SIZE - 1)
    {
        // add a null terminating byte to the buffer
        mBuffer[mBufferPos] = 0;
        // construct the syslog message (see https://tools.ietf.org/html/rfc5424#page-19)
        String syslogMessage = "<" + String(FacilityCode * 8 + mLevel) + ">1 - " + mSystem + " " + mContext + " - - - " + mColor + String((const char *)mBuffer);
        // send the syslog message to logger using UDP
        mWifiUdp->beginPacket(mHost.c_str(), mPort);
        mWifiUdp->write((const uint8_t *)syslogMessage.c_str(), syslogMessage.length());
        mWifiUdp->endPacket();
        // send the message to the Serial port
        Serial.println((const char *)mBuffer);
        mBufferPos = 0;
    }
    else
    {
        // buffer the character up for sending later
        mBuffer[mBufferPos] = c;
        mBufferPos++;
    }
    return 1;
}