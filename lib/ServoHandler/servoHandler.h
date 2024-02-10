#pragma once
#include "ESP32PWM.h"
#include "ESP32Servo.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"

#ifndef SERVOHANDLER_H
#define SERVOHANDLER_H

#define SERVO_SPEED 60
#define SERVO_MIDDLE 90
#endif

class ServoHandler
{
public:
    ServoHandler(int pin);

    bool open();
    bool close();

private:
    Servo _servo;

    bool _running = false;

    void stop();

    static void openTask(void *_this);
    static void closeTask(void *_this);

    void setPosition(int position);
};