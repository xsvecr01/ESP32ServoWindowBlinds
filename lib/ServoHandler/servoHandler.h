#pragma once
#ifndef SERVOHANDLER_H
#define SERVOHANDLER_H

#include "headers.h"

#define SERVO_SPEED 60
#define SERVO_MIDDLE 90
#define SERVO_SLOW 20

class ServoHandler
{
public:
    ServoHandler(int pin);

    bool open();
    bool close();

private:
    Servo _servo;

    bool _running = false;
    // adjust this constants by the installed blinds
    // default with speed 60:
    // open 440ms
    // close 500ms
    static const int _delay_open = 450;
    static const int _delay_close = 420;
    static const int _delay_slow = 50;

    static void openTask(void *_this);
    static void closeTask(void *_this);

    void stop();
    void moveSlow(bool open);
    void openSmooth();
    void closeSmooth();
};

#endif