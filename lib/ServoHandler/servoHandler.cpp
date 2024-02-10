#include "servoHandler.h"

ServoHandler::ServoHandler(int pin)
{
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);

    this->_servo.attach(pin);
    this->_servo.setPeriodHertz(50);
    this->stop();
}

void ServoHandler::open()
{
    xTaskCreate(
        this->openTaskImpl,
        "servo_open_task",
        512,
        this,
        5,
        NULL);
}

void ServoHandler::close()
{
    xTaskCreate(
        this->closeTaskImpl,
        "servo_close_task",
        512,
        this,
        5,
        NULL);
}

bool ServoHandler::isRunning()
{
    return this->_running;
}

void ServoHandler::stop()
{
    this->_servo.write(90);
}

void ServoHandler::openTask()
{
    this->setPosition(SERVO_FORWARD);
}
void ServoHandler::openTaskImpl(void *_this)
{
    ((ServoHandler *)_this)->openTask();
}

void ServoHandler::closeTask()
{
    this->setPosition(SERVO_BACKWARD);
}
void ServoHandler::closeTaskImpl(void *_this)
{
    ((ServoHandler *)_this)->closeTask();
}

void ServoHandler::setPosition(int servoPosition)
{
    if (this->_running == false)
    {
        this->_running = true;

        this->_servo.write(160);
        vTaskDelay(SERVO_DURATION / portTICK_RATE_MS);

        this->stop();
        this->_running = false;
    }
}