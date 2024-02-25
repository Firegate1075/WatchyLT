#pragma once

#include <Rtc_Pcf8563.h>
#include <constants.h>

typedef struct {
    uint8_t Second;
    uint8_t Minute;
    uint8_t Hour;
    uint8_t Weekday;
    uint8_t Day;
    uint8_t Month;
    uint8_t Year;
} pcfTime;

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
        Serial.println("PCF8563 get instanace");
        Serial.flush();
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
    char* formatTime();
};