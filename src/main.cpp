#include <Arduino.h>


#include "utils.h"
#include "ESP32PWM.h"
#include "ESP32Servo.h"
#include "SystemDirector.h"

// AsyncWebServer server(80);
// Servo servo;
// StateFileHandler stateHandler("/state.csv");

SystemDirector director;

void setup()
{
    Serial.begin(115200);

    pinMode(LED_BUILTIN, OUTPUT);

    // ESP32PWM::allocateTimer(0);
    // ESP32PWM::allocateTimer(1);
    // ESP32PWM::allocateTimer(2);
    // ESP32PWM::allocateTimer(3);

    // servo.attach(27);
    // servo.setPeriodHertz(50);
    // servo.write(90);

    initWifi();
    initTime();
    initSPIFFS();
    initMDNS();

    director.begin();
}

void loop()
{
    // while (Serial.available())
    // {
    //     String input = Serial.readStringUntil('\n');
    //     Serial.println("input: " + input);
    //     long value = input.toInt();
    //     servo.write(value);
    // }
}
