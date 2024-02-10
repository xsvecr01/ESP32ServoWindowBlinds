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

    void refresh();

private:
    AsyncWebServer _server;
    StateFileHandler _fileHandler;
    ServoHandler _servoHandler;

    void setupAndRunServer();

    void open();
    void close();
    bool toggle();
};

#endif