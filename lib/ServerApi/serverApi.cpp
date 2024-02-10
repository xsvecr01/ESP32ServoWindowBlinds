
#include "serverApi.h"

const char *PARAM_TIME = "time";

void handleLoadHTML(AsyncWebServerRequest *request)
{
    request->send(SPIFFS, "/index.html", "text/html");
}

void handleGetServerIp(AsyncWebServerRequest *request)
{
    request->send(200, "text/text", WiFi.localIP().toString());
}

void handleGetServerTime(AsyncWebServerRequest *request)
{
    struct tm time;
    getServerTime(&time);
    request->send(200, "text/text", formatTimeStructToString(time, "%A;%U;%B; %d %Y;%H:%M"));
}

void handleGetOpenState(AsyncWebServerRequest *request, bool open)
{
    String resp = String(open);
    request->send(200, "text/text", resp);
}

void handleGetOpenTime(AsyncWebServerRequest *request, String time)
{
    request->send(200, "text/text", time);
}

void handleGetCloseTime(AsyncWebServerRequest *request, String time)
{
    request->send(200, "text/text", time);
}

void handlePostOpenState(AsyncWebServerRequest *request, std::function<bool()> callback)
{
    String resp = String(callback());
    request->send(200, "text/text", resp);
}

void handlePostOpenTime(AsyncWebServerRequest *request, StateFileHandler *fileHandler)
{
    if (request->hasParam(PARAM_TIME, true))
    {
        String value = request->getParam(PARAM_TIME, true)->value();
        String updatedTime = fileHandler->setOpenTime(value);
        request->send(200, "text/plain", updatedTime);
    }
    else
    {
        request->send(200, "text/plain", "ERR: Time was not set.");
    }
}

void handlePostCloseTime(AsyncWebServerRequest *request, StateFileHandler *fileHandler)
{
    if (request->hasParam(PARAM_TIME, true))
    {
        String value = request->getParam(PARAM_TIME, true)->value();
        String updatedTime = fileHandler->setCloseTime(value);
        request->send(200, "text/plain", updatedTime);
    }
    else
    {
        request->send(200, "text/plain", "ERR: Time was not set.");
    }
}