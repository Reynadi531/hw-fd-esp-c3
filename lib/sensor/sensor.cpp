#include <Arduino.h>
#include "sensor.h"
#include <Wire.h>
#include <EEPROM.h>

Sensor::Sensor()
{
    this->accelerometer = nullptr;
    this->gyroscope = nullptr;

    Wire.begin();
}

void Sensor::initAccelerometer(u8_t address)
{
    this->accelerometer = new ADXL345_WE(address);
    if (!this->accelerometer->init())
    {
        Serial.println("Failed to initialize ADXL345 accelerometer!");
        return;
    }

    this->accelerometer->setFullRes(true);
    this->loadCalibrationAccel();
}

ADXL345_WE *Sensor::getAccelerometer()
{
    return this->accelerometer;
}

xyzFloat Sensor::getRawValuesAccel()
{
    xyzFloat raw;
    this->accelerometer->getRawValues(&raw);
    return raw;
}

void Sensor::initGyroscope(u8_t address)
{
    Wire.begin();
    this->gyroscope = new ITG3200();

    this->gyroscope->initialize();
    if (!this->gyroscope->testConnection())
    {
        Serial.println("Failed to connect to ITG3200 gyroscope!");
        return;
    }
    this->loadCalibrationGyro();
}

void Sensor::loadCalibrationAccel()
{
    EEPROM.get(3 * sizeof(float), this->correctionAccel[0]);
    EEPROM.get(4 * sizeof(float), this->correctionAccel[1]);
    EEPROM.get(5 * sizeof(float), this->correctionAccel[2]);
    EEPROM.get(6 * sizeof(float), this->correctionAccel[3]);
    EEPROM.get(7 * sizeof(float), this->correctionAccel[4]);
    EEPROM.get(8 * sizeof(float), this->correctionAccel[5]);

    if (this->accelerometer)
    {
        this->accelerometer->setCorrFactors(
            this->correctionAccel[0], this->correctionAccel[1],
            this->correctionAccel[2], this->correctionAccel[3],
            this->correctionAccel[4], this->correctionAccel[5]);
    }
    else
    {
        Serial.println("Accelerometer not initialized!");
    }
}

void Sensor::setCorrectionFactorAccelerometer(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax)
{
    EEPROM.put(3 * sizeof(float), xMin);
    EEPROM.put(4 * sizeof(float), xMax);
    EEPROM.put(5 * sizeof(float), yMin);
    EEPROM.put(6 * sizeof(float), yMax);
    EEPROM.put(7 * sizeof(float), zMin);
    EEPROM.put(8 * sizeof(float), zMax);
    EEPROM.commit();

    this->loadCalibrationAccel();
}

ITG3200 *Sensor::getGyroscope()
{
    return this->gyroscope;
}

char *Sensor::getSensorRawForCalibration()
{
    char buffer[100];

    if (!this->accelerometer)
    {
        Serial.println("Accelerometer not initialized!");
        return strdup(buffer);
    }

    if (!this->gyroscope)
    {
        Serial.println("Gyroscope not initialized!");
        return strdup(buffer);
    }

    xyzFloat accel;
    this->accelerometer->getRawValues(&accel);

    int16_t gy, gx, gz;
    this->gyroscope->getRotation(&gx, &gy, &gz);

    snprintf(buffer, sizeof(buffer), "%d,%d,%d;%d,%d,%d",
             (int)accel.x, (int)accel.y, (int)accel.z, gx, gy, gz);

    return strdup(buffer);
}

void Sensor::loadCalibrationGyro()
{
    EEPROM.get(0, this->correctionGyro[0]);
    EEPROM.get(sizeof(float), this->correctionGyro[1]);
    EEPROM.get(2 * sizeof(float), this->correctionGyro[2]);
}

void Sensor::setOffsetGyroscope(float xOffset, float yOffset, float zOffset)
{
    EEPROM.put(0, xOffset);
    EEPROM.put(sizeof(float), yOffset);
    EEPROM.put(2 * sizeof(float), zOffset);
    EEPROM.commit();

    this->loadCalibrationGyro();
}

xyzFloat Sensor::getCoorectedValuesAccel()
{
    xyzFloat corrected;

    this->accelerometer->getCorrectedRawValues(&corrected);

    return corrected;
}

xyzFloat Sensor::getCoorectedValuesGyro()
{
    xyzFloat corrected;
    int16_t x, y, z;

    this->gyroscope->getRotation(&x, &y, &z);

    corrected.x = (float)x - this->correctionGyro[0];
    corrected.y = (float)y - this->correctionGyro[1];
    corrected.z = (float)z - this->correctionGyro[2];

    return corrected;
}