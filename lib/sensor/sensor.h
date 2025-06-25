#include <Arduino.h>
#include <ADXL345_WE.h>
#include <ITG3200.h>

class Sensor
{
private:
    ADXL345_WE *accelerometer;
    ITG3200 *gyroscope;
    float correctionGyro[3];
    float correctionAccel[6];
public:
    void initAccelerometer(u8_t address = 0x53);
    void initGyroscope(u8_t address = 0x68);
    xyzFloat getRawValuesAccel();
    ADXL345_WE* getAccelerometer(); 
    xyzFloat getCoorectedValuesAccel();
    xyzFloat getCoorectedValuesGyro();
    char* getSensorRawForCalibration();
    void setCorrectionFactorAccelerometer(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax);
    void setOffsetGyroscope(float xOffset, float yOffset, float zOffset);
    void loadCalibrationGyro();
    void loadCalibrationAccel();
    xyzFloat checkCurrentCalibrationGyro();
    xyzFloat checkCurrentCalibrationAccel();
    ITG3200* getGyroscope();
    Sensor();
};