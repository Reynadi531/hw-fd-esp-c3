#include <Arduino.h>
#include <sensor.h>
#include <EEPROM.h>
#include <WiFi.h>
#include <time_helper.h>
#include <logger.h>

#define PIN_TRIGGER_LOGGING 0 

Sensor sensor;
bool continousData = false;
bool continousDataGyro = false;
bool continousDataAccel = false;
int recordCount = 0;

const char *WIFI_SSID = "ONE";
const char *WIFI_PASSWORD = "kanan123";

TimeHelper timeHelper;
Logger logger;

void getGyroData()
{
    int16_t x, y, z;
    sensor.getGyroscope()->getRotation(&x, &y, &z);
    Serial.print(x);
    Serial.print(",");
    Serial.print(y);
    Serial.print(",");
    Serial.println(z);
}

void getAccelData()
{
    xyzFloat accel;
    sensor.getAccelerometer()->getRawValues(&accel);
    Serial.print(accel.x);
    Serial.print(",");
    Serial.print(accel.y);
    Serial.print(",");
    Serial.println(accel.z);
}

void setup()
{
    Serial.begin(115200);
    EEPROM.begin(10 * sizeof(float)); // 3 for gyro, 6 for accelerometer, 1 for record count
    sensor.initAccelerometer();
    sensor.initGyroscope();

    pinMode(PIN_TRIGGER_LOGGING, INPUT);

    recordCount = EEPROM.read(10 * sizeof(float));
    
    if (digitalRead(PIN_TRIGGER_LOGGING) == LOW)
    {
        EEPROM.write(10 * sizeof(float), recordCount + 1); 
        EEPROM.commit();
        recordCount = EEPROM.read(10 * sizeof(float));
    }

    Serial.printf("Connecting to WiFi SSID: %s\n", WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    timeHelper.setGmtOffset(7 * 3600);
    timeHelper.intializeTime();
}

void loop()
{
    if (Serial.available() > 0)
    {
        String cmd = Serial.readStringUntil('\n');
        cmd.trim();
        if (cmd.startsWith("setCorrectionGyro "))
        {
            float x, y, z;
            int parsed = sscanf(cmd.c_str(), "setCorrectionGyro %f %f %f", &x, &y, &z);
            if (parsed == 3)
            {
                sensor.setOffsetGyroscope(x, y, z);
                Serial.println("OK");
            }
            else
            {
                Serial.println("Invalid arguments");
            }
        }
        else if (cmd.startsWith("setCorrectionAccel "))
        {
            float xMin, xMax, yMin, yMax, zMin, zMax;
            int parsed = sscanf(cmd.c_str(), "setCorrectionAccel %f %f %f %f %f %f", &xMin, &xMax, &yMin, &yMax, &zMin, &zMax);
            if (parsed == 6)
            {
                sensor.setCorrectionFactorAccelerometer(xMin, xMax, yMin, yMax, zMin, zMax);
                Serial.println("OK");
            }
            else
            {
                Serial.println("Invalid arguments");
            }
        }
        else if (cmd == "cad") {
            xyzFloat accel = sensor.getCoorectedValuesAccel();
            Serial.print(accel.x);
            Serial.print(",");
            Serial.print(accel.y);
            Serial.print(",");
            Serial.println(accel.z);
        }
        else if (cmd == "cgd")
        {
            xyzFloat gyro = sensor.getCoorectedValuesGyro();
            Serial.print(gyro.x);
            Serial.print(",");
            Serial.print(gyro.y);
            Serial.print(",");
            Serial.println(gyro.z);
        }
        else if (cmd == "cal")
        {
            sensor.loadCalibrationGyro();
            sensor.loadCalibrationAccel();
            Serial.println("Calibration loaded");
        }
        else if (cmd == "r")
        {
            continousData = false;
            continousDataGyro = false;
            continousDataAccel = false;
        }
        else if (cmd == "gd")
        {
            getGyroData();
        }
        else if (cmd == "ad")
        {
            getAccelData();
        }
        else if (cmd == "gc")
        {
            continousDataGyro = !continousDataGyro;
        }
        else if (cmd == "ac")
        {
            continousDataAccel = !continousDataAccel;
        }
        else if (cmd == "c")
        {
            continousData = true;
        }
        else if (cmd == "s")
        {
            continousData = false;
        }
    }

    if (continousData)
    {
        char *buffer = sensor.getSensorRawForCalibration();
        Serial.println(buffer);
        delay(100);
    }

    if (continousDataGyro && !continousData)
    {
        getGyroData();
        delay(100);
    }

    if (continousDataAccel && !continousData)
    {
        getAccelData();
        delay(100);
    }

    if (digitalRead(PIN_TRIGGER_LOGGING) == LOW)
    {
        if (timeHelper.CheckIsTimeInitialized())
        {
            String currentDate = timeHelper.getCurrentDate();
            logger.setDate((char*)currentDate.c_str());
            logger.setNumberOfRuns(recordCount);
            
            xyzFloat accel = sensor.getCoorectedValuesAccel();
            xyzFloat gyro = sensor.getCoorectedValuesGyro();
            
            logger.logGyroAccelData(gyro.x, gyro.y, gyro.z, accel.x, accel.y, accel.z);
            Serial.println("Data logged successfully");
        }
        else
        {
            Serial.println("Time not initialized, cannot log data");
        }
    }
}