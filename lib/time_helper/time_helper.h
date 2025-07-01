#include <Arduino.h>
#include <time.h>

class TimeHelper
{
    private:
        int gmtOffset = 0;
        bool isTimeInitialized = false;
    public:
        bool CheckIsTimeInitialized(); 
        void intializeTime();
        void setGmtOffset(int offset);
        int getGmtOffset();
        long getUnixTime();
        String getCurrentDate();
        String getFullDateTime();
        String getIsoDateTime();
};