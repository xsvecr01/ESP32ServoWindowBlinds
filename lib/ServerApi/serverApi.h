#pragma once

#ifndef SERVERAPI_H
#define SERVERAPI_H

#include "headers.h"
#include "utils.h"
#include "stateFileHandler.h"

void handleLoadHTML(AsyncWebServerRequest *request);

void handleGetServerIp(AsyncWebServerRequest *request);
void handleGetServerTime(AsyncWebServerRequest *request);
void handleGetOpenState(AsyncWebServerRequest *request, bool open);
void handleGetOpenTime(AsyncWebServerRequest *request, String time);
void handleGetCloseTime(AsyncWebServerRequest *request, String time);

void handlePostTime(AsyncWebServerRequest *request, std::function<String(String)> callback);
void handlePostOpenState(AsyncWebServerRequest *request, std::function<bool()> callback);

void setupAndRunServer(AsyncWebServer *server, StateFileHandler *state);

#endif