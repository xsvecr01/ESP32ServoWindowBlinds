#include "stateFileHandler.h"

StateFileHandler::StateFileHandler(const char *path)
{
    this->_path = path;
}

void StateFileHandler::init()
{
    openRead();
    this->_open = this->convertStringToBool(_file.readStringUntil(','));
    this->_manualMode = this->convertStringToBool(_file.readStringUntil(','));
    this->_openTime = _file.readStringUntil(',');
    this->_closeTime = _file.readStringUntil(',');
    close();
}

bool StateFileHandler::getOpenState()
{
    return this->_open;
}
bool StateFileHandler::getManualMode()
{
    return this->_manualMode;
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

bool StateFileHandler::toggleManualMode()
{
    this->_manualMode = !this->_manualMode;
    saveState();
    return this->_manualMode;
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
    String value = String(
        convertBoolToString(this->_open) + "," +
        convertBoolToString(this->_manualMode) + "," +
        this->_openTime + "," +
        this->_closeTime + ",");

    openWrite();
    this->_file.print(value);
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

bool StateFileHandler::convertStringToBool(String value)
{
    return (value.toInt() == 1);
}

String StateFileHandler::convertBoolToString(bool value)
{
    if (value == true)
    {
        return "1";
    }
    else
    {
        return "0";
    }
}