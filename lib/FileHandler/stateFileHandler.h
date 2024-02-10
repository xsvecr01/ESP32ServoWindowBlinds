#pragma once
#ifndef STATE_FILE_HANDLER_H
#define STATE_FILE_HANDLER_H

#include "headers.h"
#include "utils.h"

// state.csv
// state,manualMode,openTime,closeTime
// state: 1 == open, 0 == closed
// manualMode: 1 == manual (via button), 0 == automatic (by time)
// openTime: 6:00
// closeTime: 18:00

class StateFileHandler
{
public:
    StateFileHandler(const char *path);
    void init();

    bool getOpenState();
    bool getManualMode();
    String getOpenTime();
    String getCloseTime();

    bool toggleOpenState();
    bool toggleManualMode();
    String setOpenTime(String openTime);
    String setCloseTime(String closeTime);

private:
    const char *_path;
    File _file;
    bool _open;
    bool _manualMode;
    String _openTime;
    String _closeTime;

    void saveState();
    void openRead();
    void openWrite();
    void close();

    bool convertStringToBool(String value);
    String convertBoolToString(bool value);
};
#endif