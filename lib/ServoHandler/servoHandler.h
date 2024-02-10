#pragma once
#include "ESP32PWM.h"
#include "ESP32Servo.h"

#ifndef SERVOHANDLER_H
#define SERVOHANDLER_H
#define SERVO_DURATION 1000
#define SERVO_FORWARD 160
#define SERVO_BACKWARD 20
#endif

class ServoHandler
{
public:
    ServoHandler(int pin);

    void open();
    void close();
    bool isRunning();

private:
    Servo _servo;

    bool _running = false;

    void stop();

    void openTask();
    static void openTaskImpl(void *_this);
    
    void closeTask();
    static void closeTaskImpl(void *_this);
    
    void setPosition(int servoPosition);
};