#pragma once
#ifndef SERVOHANDLER_H
#define SERVOHANDLER_H

#include "headers.h"

#define SERVO_SPEED 60
#define SERVO_MIDDLE 90

class ServoHandler
{
public:
    ServoHandler(int pin);

    bool open();
    bool close();

private:
    Servo _servo;

    bool _running = false;
    static const int _servoDuration = 500;

    void stop();

    static void openTask(void *_this);
    static void closeTask(void *_this);

    void setPosition(int position);
};

#endif