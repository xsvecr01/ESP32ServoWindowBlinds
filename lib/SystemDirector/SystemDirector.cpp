#include "SystemDirector.h"

SystemDirector::SystemDirector() : _server(80), _fileHandler("/state.csv"), _servoHandler(27)
{
}

void SystemDirector::begin()
{
    this->_fileHandler.init();
    this->setupAndRunServer();

    xTaskCreate(
        this->refreshTask,
        "refresh_task",
        4096,
        (void *)this,
        1,
        NULL);
}

void SystemDirector::setupAndRunServer()
{
    this->_server.on("/", HTTP_GET, handleLoadHTML);

    this->_server.on("/server-ip", HTTP_GET, handleGetServerIp);
    this->_server.on("/server-time", HTTP_GET, handleGetServerTime);
    this->_server.on("/open-state", HTTP_GET, [this](AsyncWebServerRequest *request)
                     { handleGetBoolState(request, this->_fileHandler.getOpenState()); });
    this->_server.on("/manual-state", HTTP_GET, [this](AsyncWebServerRequest *request)
                     { handleGetBoolState(request, this->_fileHandler.getManualMode()); });

    this->_server.on("/time-open", HTTP_GET, [this](AsyncWebServerRequest *request)
                     { handleGetOpenTime(request, this->_fileHandler.getOpenTime()); });

    this->_server.on("/time-close", HTTP_GET, [this](AsyncWebServerRequest *request)
                     { handleGetCloseTime(request, this->_fileHandler.getCloseTime()); });

    this->_server.on("/open-state", HTTP_POST, [this](AsyncWebServerRequest *request)
                     { handlePostBoolState(request, [this]()
                                           { return this->toggle(); }); });

    this->_server.on("/manual-state", HTTP_POST, [this](AsyncWebServerRequest *request)
                     { handlePostBoolState(request, [this]()
                                           { return this->toggleMode(); }); });

    this->_server.on("/time-open", HTTP_POST, [this](AsyncWebServerRequest *request)
                     { handlePostTime(request, [this](String time)
                                      { return this->_fileHandler.setOpenTime(time); }); });

    this->_server.on("/time-close", HTTP_POST, [this](AsyncWebServerRequest *request)
                     { handlePostTime(request, [this](String time)
                                      { return this->_fileHandler.setCloseTime(time); }); });

    this->_server.begin();
}

void SystemDirector::open()
{
    if (this->_servoHandler.open())
    {
        this->_fileHandler.toggleOpenState();
        Serial.println("Director: blinds open");
    }
}

void SystemDirector::close()
{
    if (this->_servoHandler.close())
    {
        this->_fileHandler.toggleOpenState();
        Serial.println("Director: blinds closed");
    }
}

bool SystemDirector::toggle()
{
    if (this->_fileHandler.getOpenState() == true)
    {
        this->close();
    }
    else
    {
        this->open();
    }
    return this->_fileHandler.getOpenState();
}

bool SystemDirector::toggleMode()
{
    return this->_fileHandler.toggleManualMode();
}

void SystemDirector::refresh()
{
    if (this->_fileHandler.getManualMode() == true)
    {
        return;
    }

    if (isOpenTime(this->_fileHandler.getOpenTime(), this->_fileHandler.getCloseTime()))
    {
        if (this->_fileHandler.getOpenState() == false)
        {
            this->open();
        }
    }
    else
    {
        if (this->_fileHandler.getOpenState() == true)
        {
            this->close();
        }
    }
}

void SystemDirector::refreshTask(void *_this)
{
    while (1)
    {
        ((SystemDirector *)_this)->refresh();
        vTaskDelay(((SystemDirector *)_this)->_refreshDelay / portTICK_RATE_MS);
    }
}