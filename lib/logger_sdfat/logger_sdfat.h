#include <Arduino.h>
#include <SdFat.h>

#define sdConfig SdSpiConfig(SS, DEDICATED_SPI, SD_SCK_MHZ(50))

class Logger
{
private:
    char* currentDate;
    int numberOfRuns = 0;
    SdFat32 sd;
    File32 file;
public:
    Logger();
    void initialize();
    void setDate(char* date);
    void logGyroAccelData(float gyroX, float gyroY, float gyroZ, float accelX, float accelY, float accelZ, String timestamp);
    void createDirectoryForDate();
    void setNumberOfRuns(int runs);
};
