#pragma once
#include "Repositories/CredentialRepository.h"
#include "Repositories/StateRepository.h"
#include "Services/BMA456.h"
#include "Services/GPIOHandler.h"
#include "Services/PCF8563.h"
#include "Services/WifiHandler.h"
#include "View/ConfigPortalView.h"
#include "View/StepView.h"
#include "View/WatchFace.h"
#include "View/WatchyDisplay.h"
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

    PCF8563& rtc;
    GPIOHandler& gpio;
    StateRepository& stateRepo;

    bool m_viewChanged;
    bool m_busy = false;

protected:
public:
    // Get singleton instance of Controller class
    static Controller& getInstance();

    void handleWakeup();
    void handleButtons();
    void handleRadio();
    void updateScreen();
    void sleep();
    bool isBusy();

    // remove copy constructor and assignment operator
    Controller(const Controller&) = delete;
    void operator=(const Controller&) = delete;
};