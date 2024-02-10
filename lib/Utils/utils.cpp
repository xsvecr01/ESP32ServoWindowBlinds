#include "utils.h"

const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;

class BlindsServo
{

public:
    BlindsServo(int pin)
    {
        ESP32PWM::allocateTimer(0);
        ESP32PWM::allocateTimer(1);
        ESP32PWM::allocateTimer(2);
        ESP32PWM::allocateTimer(3);

        this->servo.setPeriodHertz(50);
        this->servo.attach(pin);
        this->servo.write(180);
    }

    void calibrate()
    {
        this->servo.write(45);
        delay(1000);
        this->servo.write(135);
        delay(1000);
        this->servo.write(90);
    }

private:
    Servo servo;
};

void initGPIO()
{
    // BlindsServo servo(SERVO);
    // servo.calibrate();
    pinMode(LED_BUILTIN, OUTPUT);
}

void initWifi()
{
    WiFi.mode(WIFI_STA);
    WiFi.setAutoReconnect(true);

    WiFi.begin(WIFI_SSID, WIFI_PASS);
    Serial.print("Connecting to WiFi ..");

    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print('.');
        digitalWrite(LED_BUILTIN, HIGH);
        delay(500);
        digitalWrite(LED_BUILTIN, LOW);
        delay(500);
    }
    Serial.println("");
    Serial.print("Connected with IP address: ");
    Serial.println(WiFi.localIP());
}

void initSPIFFS()
{
    if (!SPIFFS.begin(true))
    {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }
}

void initMDNS()
{
    if (!MDNS.begin("zaluzie"))
    {
        Serial.println("MDNS responder is not running.");
        return;
    }

    MDNS.addService("http", "tcp", 80);
}

void getServerTime(struct tm *info)
{
    if (!getLocalTime(info))
    {
        Serial.println("Failed to obtain time");
        return;
    }
}

String formatTimeStructToString(struct tm info, const char *format)
{
    char buffer[80];
    strftime(buffer, sizeof(buffer), format, &info);
    return buffer;
}

void initTime()
{
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    struct tm time;
    getServerTime(&time);
    String timeString = formatTimeStructToString(time, "%A, %B %d %Y %H:%M:%S zone %Z %z ");
    Serial.print("Server time: ");
    Serial.println(timeString);
}

int getServerMinutesTotal()
{
    struct tm now;
    getServerTime(&now);
    return (now.tm_hour * 60) + now.tm_min;
}

int getMinutesTotal(String time)
{
    int hr = time.substring(0, 2).toInt();
    int min = time.substring(3).toInt();

    return (hr * 60) + min;
}

bool isOpenTime(String openTime, String closeTime)
{
    int openMinutesTotal = getMinutesTotal(openTime);
    int closeMinutesTotal = getMinutesTotal(closeTime);
    int serverMinutesTotal = getServerMinutesTotal();

    if (openMinutesTotal > closeMinutesTotal)
    {
        return (serverMinutesTotal >= closeMinutesTotal) || (serverMinutesTotal < openMinutesTotal);
    }
    else
    {
        return (openMinutesTotal <= serverMinutesTotal) && (serverMinutesTotal <= closeMinutesTotal);
    }
}