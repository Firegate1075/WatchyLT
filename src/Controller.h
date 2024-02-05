#pragma once
#include "Services/BMA456.h"
#include "Services/PCF8563.h"
#include "View/WatchyDisplay.h"
#include "View/Metropolis_Font.h"
#include "constants.h"
#include <Arduino.h>
#include <Wire.h>
#include <esp32-hal.h>
#include <esp_sleep.h>

/// @brief Controller class for Watchy
class Controller {
private:
    // private constructor and destructor
    Controller();
    ~Controller() { }

    void handleWakeup();

    PCF8563& rtc;

protected:
public:
    // Get singleton instance of Controller class
    static Controller& getInstance();

    // remove copy constructor and assignment operator
    Controller(const Controller&) = delete;
    void operator=(const Controller&) = delete;
};