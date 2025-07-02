#include <Arduino.h>
#include <logger_sdfat.h>
#include <SdFat.h>

Logger::Logger()
{
    this->currentDate = nullptr;
    this->numberOfRuns = 0;
    this->sd = SdFat32();
    this->file = File32();
}

void Logger::initialize()
{
    if(!this->sd.begin(sdConfig)) 
    {
        sd.initErrorHalt(&Serial);
        return;
    }
    Serial.println("Logger initialized with SdFat");
}

void Logger::setDate(char* date)
{
    this->currentDate = date;
}

void Logger::setNumberOfRuns(int runs)
{
    this->numberOfRuns = runs;
}

void Logger::createDirectoryForDate()
{
    if (!this->sd.exists(this->currentDate)) 
    {
        if (!this->sd.mkdir(this->currentDate)) 
        {
            Serial.println("Error creating directory for date");
        } else {
            Serial.println("Successfully created directory for date: " + String(this->currentDate));
        }
    }

}

void Logger::logGyroAccelData(float gyroX, float gyroY, float gyroZ, float accelX, float accelY, float accelZ, String timestamp)
{
    this->createDirectoryForDate();

    char fileName[50];
    snprintf(fileName, sizeof(fileName), "/%s/%s_%d.txt", this->currentDate, this->currentDate, this->numberOfRuns);

    if (!this->sd.exists(fileName)) 
    {
        this->file = this->sd.open(fileName, FILE_WRITE);
        if (!this->file) 
        {
            Serial.println("Error creating data file");
            return;
        }
        this->file.println("Timestamp,GyroX,GyroY,GyroZ,AccelX,AccelY,AccelZ"); // Header
        this->file.close();
    }

    this->file = this->sd.open(fileName, O_RDWR | O_CREAT | O_APPEND);
    if (!this->file) 
    {
        Serial.println("Error opening data file");
        return;
    }

    String logEntry = timestamp + "," + (gyroX) + "," + String(gyroY) + "," + String(gyroZ) + "," +
                      String(accelX) + "," + String(accelY) + "," + String(accelZ);

    Serial.println("Logging data: " + logEntry);

    this->file.println(logEntry);
    this->file.close();
}

