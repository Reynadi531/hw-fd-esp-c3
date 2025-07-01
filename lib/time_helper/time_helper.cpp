#include <Arduino.h>
#include "time_helper.h"
#include <time.h>
#include <WiFi.h>

void TimeHelper::setGmtOffset(int offset)
{
    this->gmtOffset = offset;
}

void TimeHelper::intializeTime()
{
    configTime(this->gmtOffset, 0, "pool.ntp.org");
    struct tm timeinfo;
    while(!getLocalTime(&timeinfo, 1000))
    {
        Serial.println("Failed to obtain time");
        delay(1000);
    }

    Serial.println("Time initialized");
    this->isTimeInitialized = true;
}

long TimeHelper::getUnixTime()
{
    if (!this->isTimeInitialized)
    {
        Serial.println("Time not initialized");
        return 0;
    }

    struct tm timeinfo;
    if (!getLocalTime(&timeinfo, 1000))
    {
        Serial.println("Failed to obtain time");
        return 0;
    }

    return mktime(&timeinfo);
}

bool TimeHelper::CheckIsTimeInitialized()
{
    return this->isTimeInitialized;
}

String TimeHelper::getCurrentDate()
{
    if (!this->isTimeInitialized)
    {
        Serial.println("Time not initialized");
        return "";
    }

    struct tm timeinfo;
    if (!getLocalTime(&timeinfo, 1000))
    {
        Serial.println("Failed to obtain time");
        return "";
    }

    char buffer[30];
    strftime(buffer, sizeof(buffer), "%Y%m%d", &timeinfo);
    return String(buffer);
}

String TimeHelper::getFullDateTime()
{
    if (!this->isTimeInitialized)
    {
        Serial.println("Time not initialized");
        return "";
    }

    struct tm timeinfo;
    if (!getLocalTime(&timeinfo, 1000))
    {
        Serial.println("Failed to obtain time");
        return "";
    }

    char buffer[30];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
    return String(buffer);
}

String TimeHelper::getIsoDateTime()
{
    if (!this->isTimeInitialized)
    {
        Serial.println("Time not initialized");
        return "";
    }

    struct tm timeinfo;
    if (!getLocalTime(&timeinfo, 1000))
    {
        Serial.println("Failed to obtain time");
        return "";
    }

    char buffer[30];
    strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%S", &timeinfo);
    return String(buffer);
}