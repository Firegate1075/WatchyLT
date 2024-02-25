#pragma once
#include "Services/BMA456.h"
#include "Services/GPIOHandler.h"
#include "Services/PCF8563.h"
#include "View/Metropolis_Font.h"
#include "View/View.h"
#include "constants.h"
#include <Arduino.h>
#include <Wire.h>
#include <esp32-hal.h>
#include <esp_sleep.h>

// DEBUGGING
#define DEBUG

#ifdef DEBUG
#define debugBegin() Serial.begin(115200)
#define debugPrint(x) \
    Serial.print(x);  \
    Serial.flush();
#define debugPrintln(x) \
    Serial.println(x);  \
    Serial.flush();
#else
#define debugBegin()
#define debugPrint(x)
#define debugPrintln(x)
#endif

/// @brief Controller class for Watchy
class Controller {
private:
    // private constructor and destructor
    Controller();
    ~Controller() { }

    void handleWakeup();

    PCF8563& rtc;
    GPIOHandler& gpio;
    View& viewObj;

protected:
public:
    // Get singleton instance of Controller class
    static Controller& getInstance();

    // remove copy constructor and assignment operator
    Controller(const Controller&) = delete;
    void operator=(const Controller&) = delete;
};