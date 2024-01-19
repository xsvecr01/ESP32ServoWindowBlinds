#include "SystemDirector.h"

SystemDirector::SystemDirector() : _server(80), _fileHandler("/state.csv")
{
}

void SystemDirector::begin()
{
    this->_fileHandler.init();
    this->setupAndRunServer();
    this->refresh();
}

void SystemDirector::refresh()
{

    // is it open?
    if (this->_fileHandler.getOpenState())
    {
        // should it close - server time is bigger than closeTime
        if (compareTime(this->_fileHandler.getCloseTime()))
        {
            this->closeBlinds();
        }
    }
    else if (!this->_fileHandler.getOpenState())
    {
        if (compareTime(this->_fileHandler.getOpenTime()))
        {
            this->openBlinds();
        }
    }
}

void SystemDirector::setupAndRunServer()
{
    this->_server.on("/", HTTP_GET, handleLoadHTML);

    this->_server.on("/server-time", HTTP_GET, handleGetServerTime);

    this->_server.on("/open-state", HTTP_GET, [this](AsyncWebServerRequest *request)
                     { handleGetOpenState(request, this->_fileHandler.getOpenState()); });

    this->_server.on("/time-open", HTTP_GET, [this](AsyncWebServerRequest *request)
                     { handleGetOpenTime(request, this->_fileHandler.getOpenTime()); });

    this->_server.on("/time-close", HTTP_GET, [this](AsyncWebServerRequest *request)
                     { handleGetCloseTime(request, this->_fileHandler.getCloseTime()); });

    this->_server.on("/open-state", HTTP_POST, [this](AsyncWebServerRequest *request)
                     { handlePostOpenState(request, &this->_fileHandler); });

    this->_server.on("/time-open", HTTP_POST, [this](AsyncWebServerRequest *request)
                     { handlePostOpenTime(request, &this->_fileHandler); });

    this->_server.on("/time-close", HTTP_POST, [this](AsyncWebServerRequest *request)
                     { handlePostCloseTime(request, &this->_fileHandler); });

    this->_server.begin();
}

void SystemDirector::openBlinds()
{
    this->_fileHandler.toggleOpenState();
    // todo
}

void SystemDirector::closeBlinds()
{
    this->_fileHandler.toggleOpenState();
}