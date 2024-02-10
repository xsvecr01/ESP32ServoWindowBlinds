#pragma once
#ifndef UTILS_H
#define UTILS_H

#include "headers.h"
#include "wifiCredentials.h"

// create a new file in this folder named wifiCredentials.h
// define following credentials in wifiCredentials.h file
// #define WIFI_SSID "wifi_name"
// #define WIFI_PASS "wifi_password"

void initGPIO();
void initWifi();
void initSPIFFS();
void initMDNS();

void getServerTime(struct tm *info);

/// @brief Format time structure into string
/// @param info time structure: struct tm time
/// @param format return format: "%A, %B %d %Y %H:%M:%S zone %Z %z " - Wednesday, January 17 2024 17:50:54 zone UTC +0100
/// @param format "%H:%M" - hour:minute format
/// @return String - formatted time
String formatTimeStructToString(struct tm info, const char *format);

void initTime();

/// @brief compare time in HH:MM format with local server time
/// @param time String HH:MM format
/// @return true if local server time is bigger than time, false otherwise
bool compareTime(String time);
#endif