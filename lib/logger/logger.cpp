#include <Arduino.h>
#include "logger.h"
#include <SD.h>

Logger::Logger()
{
    if (!SD.begin(7))
    {
        Serial.println("SD Card initialization failed!");
        return;
    }

    uint8_t cardType = SD.cardType();

    if (cardType == CARD_NONE)
    {
        Serial.println("No SD card attached");
        return;
    }

    Serial.println("Logger initialized");
}

void Logger::setNumberOfRuns(int runs)
{
    this->numberOfRuns = runs;
}

void Logger::setDate(char* dateTime)
{
    this->currentDate = dateTime; 
}

void Logger::logGyroAccelData(float gyroX, float gyroY, float gyroZ, float accelX, float accelY, float accelZ)
{
    this->createDirectoryForDate();

    char fileName[50];
    snprintf(fileName, sizeof(fileName), "./%s/%s_%d.txt", this->currentDate, this->currentDate, this->numberOfRuns);
    File dataFile = SD.open(fileName, FILE_APPEND);
    if (!dataFile)
    {
        Serial.println("Error opening data file");
        return;
    }

    String logEntry = String(gyroX) + "," + String(gyroY) + "," + String(gyroZ) + "," +
                      String(accelX) + "," + String(accelY) + "," + String(accelZ) + "\n";

    dataFile.print(logEntry);
    dataFile.close();
}

void Logger::createDirectoryForDate()
{
    if (!SD.exists(this->currentDate))
    {
        if (!SD.mkdir(this->currentDate))
  
        {
            Serial.println("Failed to create directory");
        }
    }
}
