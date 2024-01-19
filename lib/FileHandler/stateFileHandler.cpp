
#include "stateFileHandler.h"

StateFileHandler::StateFileHandler(const char *path)
{
    this->_path = path;
}

void StateFileHandler::init()
{
    openRead();
    String open = _file.readStringUntil(',');
    this->_openTime = _file.readStringUntil(',');
    this->_closeTime = _file.readStringUntil(',');
    close();

    if (open.toInt() == 1)
    {
        this->_open = true;
    }
    else
    {
        this->_open = false;
    }
}

bool StateFileHandler::getOpenState()
{
    return this->_open;
}
String StateFileHandler::getOpenTime()
{
    return this->_openTime;
}
String StateFileHandler::getCloseTime()
{
    return this->_closeTime;
}

bool StateFileHandler::toggleOpenState()
{
    this->_open = !this->_open;
    saveState();
    return this->_open;
}

String StateFileHandler::setOpenTime(String openTime)
{
    this->_openTime = openTime;
    saveState();
    return this->_openTime;
}

String StateFileHandler::setCloseTime(String closeTime)
{
    this->_closeTime = closeTime;
    saveState();
    return this->_closeTime;
}

void StateFileHandler::saveState()
{
    String openString = "0";

    if (this->_open)
    {
        openString = "1";
    }

    openWrite();
    this->_file.print(String(openString + "," + this->_openTime + "," + this->_closeTime + ","));
    close();
}

void StateFileHandler::openRead()
{
    this->_file = SPIFFS.open(_path, FILE_READ);
    if (!this->_file)
    {
        Serial.println("Failed to open file for READ");
    }
}

void StateFileHandler::openWrite()
{
    this->_file = SPIFFS.open(_path, FILE_WRITE);
    if (!this->_file)
    {
        Serial.println("Failed to open file for WRITE");
    }
}

void StateFileHandler::close()
{
    this->_file.close();
}