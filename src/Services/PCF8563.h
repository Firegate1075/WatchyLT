#pragma once

#include <Rtc_Pcf8563.h>

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
};