#include "utils.h"

const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;

void initGPIO()
{
    pinMode(LED_BUILTIN, OUTPUT);
}

void initWifi()
{
    WiFi.mode(WIFI_STA);
    WiFi.setAutoReconnect(true);
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    waitForWifiConnection();

    Serial.println(WiFi.localIP());
}

void waitForWifiConnection()
{
    Serial.print("Connecting to WiFi");

    int tries = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
        if (tries > 60)
        {
            resetSystem("WiFi");
        }
        if (tries != 0 && tries % 20 == 0)
        {
            if (WiFi.reconnect())
            {
                Serial.println("");
                Serial.print("Reconnecting");
            }
            else
            {
                resetSystem("WiFi reconnect");
            }
        }

        Serial.print('.');
        blinkLEDBlocking(1000);

        tries++;
    }

    Serial.println("");
    Serial.print("Connected with IP address: ");
}

void initSPIFFS()
{
    if (!SPIFFS.begin(true))
    {
        resetSystem("SPIFFS");
    }
}

void initMDNS()
{
    if (!MDNS.begin("zaluzie"))
    {
        resetSystem("MDNS");
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

String getUptime()
{
    int64_t microSecondsSinceBoot = esp_timer_get_time();
    return convertMicroseconds(microSecondsSinceBoot);
}

String convertMicroseconds(long long microseconds)
{
    long long seconds = microseconds / 1000000;
    long long minutes = seconds / 60;
    long long hours = minutes / 60;
    long long days = hours / 24;

    // Calculate remaining hours, minutes, and seconds after removing days
    hours %= 24;
    minutes %= 60;
    seconds %= 60;

    // add leading zeros
    String result = "";
    result += String(days) + ":";
    if (hours < 10)
        result += "0";
    result += String(hours) + ":";
    if (minutes < 10)
        result += "0";
    result += String(minutes) + ":";
    if (seconds < 10)
        result += "0";
    result += String(seconds);

    return result;
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
        return (serverMinutesTotal >= closeMinutesTotal) || (serverMinutesTotal <= openMinutesTotal);
    }
    else
    {
        return (serverMinutesTotal >= openMinutesTotal) && (serverMinutesTotal <= closeMinutesTotal);
    }
}

void blinkLEDBlocking(int interval)
{
    int duration = interval / 2;
    digitalWrite(LED_BUILTIN, HIGH);
    delay(duration);
    digitalWrite(LED_BUILTIN, LOW);
    delay(duration);
}

void resetSystem(String source)
{
    Serial.println("Error - performing software reset. Source: " + source);
    for (int i = 0; i < 5; i++)
    {
        blinkLEDBlocking(100);
    }
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    ESP.restart();
}