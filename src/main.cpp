#include "headers.h"
#include "utils.h"
#include "SystemDirector.h"


SystemDirector director;

void setup()
{
    Serial.begin(115200);

    pinMode(LED_BUILTIN, OUTPUT);
    nvs_flash_init();

    initWifi();
    initTime();
    initSPIFFS();
    initMDNS();

    director.begin();
}

void loop() {}
