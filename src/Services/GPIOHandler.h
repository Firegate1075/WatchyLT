#pragma once

#include <Arduino.h>
#include <constants.h>

class GPIOHandler {
private:
    // private constructor and destructor
    GPIOHandler();
    ~GPIOHandler() { }

protected:
public:
    // Get singleton instance of BLEHandler class
    static GPIOHandler& getInstance()
    {
        Serial.println("GPIOHandler get instanace");
        Serial.flush();
        static GPIOHandler instance;
        return instance;
    }
    // remove copy constructor and assignment operator
    GPIOHandler(const GPIOHandler&) = delete;
    void operator=(const GPIOHandler&) = delete;

    double getBatteryVoltage();
};