#include <Arduino.h>

class Logger
{
private:
    char* currentDate;
    int numberOfRuns = 0;
public:
    Logger();
    void setDate(char* date);
    void logGyroAccelData(float gyroX, float gyroY, float gyroZ, float accelX, float accelY, float accelZ);
    void createDirectoryForDate();
    void setNumberOfRuns(int runs);
};
