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

bool ServoHandler::open()
{
    if (this->_running == true)
    {
        return false;
    }
    else
    {
        xTaskCreate(
            this->openTask,
            "open_task",
            1024,
            (void *)this,
            5,
            NULL);

        return true;
    }
}

bool ServoHandler::close()
{
    if (this->_running == true)
    {
        return false;
    }
    else
    {
        xTaskCreate(
            this->closeTask,
            "close_task",
            1024,
            (void *)this,
            5,
            NULL);

        return true;
    }
}

void ServoHandler::stop()
{
    this->_servo.write(SERVO_MIDDLE);
}

void ServoHandler::openTask(void *_this)
{
    while (1)
    {
        ((ServoHandler *)_this)->setPosition(SERVO_MIDDLE + SERVO_SPEED);
        vTaskDelete(NULL);
    }
}

void ServoHandler::closeTask(void *_this)
{
    while (1)
    {
        ((ServoHandler *)_this)->setPosition(SERVO_MIDDLE - SERVO_SPEED);
        vTaskDelete(NULL);
    }
}

void ServoHandler::setPosition(int position)
{
    if (this->_running == false)
    {
        this->_running = true;

        this->_servo.write(position);
        vTaskDelay(1000 / portTICK_RATE_MS);
        this->stop();
        this->_running = false;
    }
}