#pragma once
#ifndef STATE_FILE_HANDLER_H
#define STATE_FILE_HANDLER_H

#include "headers.h"
#include "utils.h"

// state.csv
// state,openTime,closeTime
// open 1 | closed 0
// 6:00 open time
// 18:00 closing time

class StateFileHandler
{
public:
    StateFileHandler(const char *path);
    void init();

    bool getOpenState();
    String getOpenTime();
    String getCloseTime();

    bool toggleOpenState();
    String setOpenTime(String openTime);
    String setCloseTime(String closeTime);

private:
    const char *_path;
    File _file;
    bool _open;
    String _openTime;
    String _closeTime;

    void saveState();
    void openRead();
    void openWrite();
    void close();
};
#endif