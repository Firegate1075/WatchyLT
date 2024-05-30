#pragma once

#include <Arduino.h>
#include <constants.h>

class GPIOHandler {
private:
    // private constructor and destructor
    GPIOHandler();
    ~GPIOHandler() { }
    uint8_t m_mask;

protected:
public:
    // Get singleton instance of BLEHandler class
    static GPIOHandler& getInstance()
    {
        static GPIOHandler instance;
        return instance;
    }
    // remove copy constructor and assignment operator
    GPIOHandler(const GPIOHandler&) = delete;
    void operator=(const GPIOHandler&) = delete;

    double getBatteryVoltage();
    uint8_t getButtonMask();
    uint8_t readButtons();
};