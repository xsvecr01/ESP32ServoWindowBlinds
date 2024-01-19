
#include "serverApi.h"

const char *PARAM_TIME = "time";

void handleLoadHTML(AsyncWebServerRequest *request)
{
    request->send(SPIFFS, "/index.html", "text/html");
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

void handlePostOpenState(AsyncWebServerRequest *request, StateFileHandler *fileHandler)
{
    String resp = String(fileHandler->toggleOpenState());
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

// void setupAndRunServer(AsyncWebServer *server, StateFileHandler *state)
// {
//     server->on("/", HTTP_GET, handleLoadHTML);
//     server->on("/server-time", HTTP_GET, handleGetServerTime);

//     server->on("/time-open", HTTP_GET, [state](AsyncWebServerRequest *request)
//                { handleGetOpenTime(request, state); });

//     server->on("/time-open", HTTP_POST, [state](AsyncWebServerRequest *request)
//                { handlePostOpenTime(request, state); });

//     server->on("/time-close", HTTP_GET, [state](AsyncWebServerRequest *request)
//                { handleGetCloseTime(request, state); });

//     server->on("/time-close", HTTP_POST, [state](AsyncWebServerRequest *request)
//                { handlePostCloseTime(request, state); });

//     server->begin();
// }