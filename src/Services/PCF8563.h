#pragma once

#include <Rtc_Pcf8563.h>
#include <constants.h>

class PCF8563 {
private:
    // private constructor and destructor
    PCF8563();
    ~PCF8563() { }

    Rtc_Pcf8563 rtc;

protected:
public:
    // Get singleton instance of PCF8563 class
    static PCF8563& getInstance()
    {
        static PCF8563 instance;
        return instance;
    }
    // remove copy constructor and assignment operator
    PCF8563(const PCF8563&) = delete;
    void operator=(const PCF8563&) = delete;

    void resetRTC();
    void setTimeDate(pcfTime tm);
    void getTimeDate(pcfTime& tm);
    void resetAlarm();
    void clearAlarm();
    void setAlarm(uint8_t minutesToWakeup = 1);
    char* formatTime();
};