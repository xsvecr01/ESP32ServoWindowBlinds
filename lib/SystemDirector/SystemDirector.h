#pragma once
#include "ESPAsyncWebServer.h"
#include "stateFileHandler.h"
#include "serverApi.h"

#ifndef SYSTEMDIRECTOR_H
#define SYSTEMDIRECTOR_H
#endif

class SystemDirector
{
public:
    SystemDirector();
    void begin();

private:
    AsyncWebServer _server;
    StateFileHandler _fileHandler;

    void setupAndRunServer();

    void openBlinds();
    void closeBlinds();
    void refresh();
};