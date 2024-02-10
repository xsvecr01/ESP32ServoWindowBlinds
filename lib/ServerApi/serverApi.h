#pragma once

#ifndef SERVERAPI_H
#define SERVERAPI_H
#endif

#include "ESPAsyncWebServer.h"
#include <SPIFFS.h>
#include "utils.h"
#include "stateFileHandler.h"

void handleLoadHTML(AsyncWebServerRequest *request);

void handleGetServerIp(AsyncWebServerRequest *request);
void handleGetServerTime(AsyncWebServerRequest *request);
void handleGetOpenState(AsyncWebServerRequest *request, bool open);
void handleGetOpenTime(AsyncWebServerRequest *request, String time);
void handleGetCloseTime(AsyncWebServerRequest *request, String time);

void handlePostOpenTime(AsyncWebServerRequest *request, StateFileHandler *fileHandler);
void handlePostCloseTime(AsyncWebServerRequest *request, StateFileHandler *fileHandler);
void handlePostOpenState(AsyncWebServerRequest *request, std::function<bool()> callback);

void setupAndRunServer(AsyncWebServer *server, StateFileHandler *state);