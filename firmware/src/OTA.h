#ifndef __ota__h
#define __ota__h

class PapertrailLogger;

class OTA
{
    PapertrailLogger *logger;

public:
    OTA(PapertrailLogger *logger, const char *hostname);
    void loop();
};

#endif