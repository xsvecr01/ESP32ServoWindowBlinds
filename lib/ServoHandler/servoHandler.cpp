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
            1,
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
            1,
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
        ((ServoHandler *)_this)->openSmooth();
        vTaskDelete(NULL);
    }
}

void ServoHandler::closeTask(void *_this)
{
    while (1)
    {
        ((ServoHandler *)_this)->closeSmooth();
        vTaskDelete(NULL);
    }
}

void ServoHandler::openSmooth()
{
    if (this->_running == false)
    {
        this->_running = true;
        this->moveSlow(true);

        this->_servo.write(SERVO_MIDDLE + SERVO_SPEED);
        vTaskDelay(_delay_open / portTICK_RATE_MS);

        this->moveSlow(true);

        this->stop();
        this->_running = false;
    }
}

void ServoHandler::closeSmooth()
{
    if (this->_running == false)
    {
        this->_running = true;
        this->moveSlow(false);

        this->_servo.write(SERVO_MIDDLE - SERVO_SPEED);
        vTaskDelay(_delay_close / portTICK_RATE_MS);

        this->moveSlow(false);

        this->stop();
        this->_running = false;
    }
}

void ServoHandler::moveSlow(bool open)
{
    int speed = open ? SERVO_SLOW : -SERVO_SLOW;

    this->_servo.write(SERVO_MIDDLE + speed);
    vTaskDelay(_delay_slow / portTICK_RATE_MS);
}