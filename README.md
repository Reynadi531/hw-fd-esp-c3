# Fall Detection Embeded Code for Hardware Instrument using ESP32 C3 platform

*Codename : Perth*

### Hardware: 
 - GY-85 (QMC5883L, ADXL345, ITG3205)
 - RTC Clock (DS1302)
 - MicroSD Module SPI
 - ESP32 C3 Supermini


 ### Calibration Tools:
 - [GUI Repository](https://github.com/Reynadi531/imu-cal)

### Stack
- PlatformIO

### Config the wifi
Please add the wifi_config.h under the src folder:
```cpp
const char* WIFI_SSID = "YOUR SSID HERE";
const char* WIFI_PASSWORD = "YOUR SSID PASSWORD HERE";
```