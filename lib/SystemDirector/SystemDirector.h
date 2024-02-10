#pragma once
#ifndef SYSTEMDIRECTOR_H
#define SYSTEMDIRECTOR_H

#include "headers.h"
#include "stateFileHandler.h"
#include "serverApi.h"
#include "servoHandler.h"

class SystemDirector
{
public:
    SystemDirector();
    void begin();

private:
    AsyncWebServer _server;
    StateFileHandler _fileHandler;
    ServoHandler _servoHandler;

    static const int _refreshDelay = 10000;

    void setupAndRunServer();
    void refresh();

    void open();
    void close();
    bool toggle();
    bool toggleMode();

    static void refreshTask(void *_this);
};

#endif