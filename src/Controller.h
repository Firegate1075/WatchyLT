#pragma once
#include "DebugUtils.h"
#include "Repositories/CredentialRepository.h"
#include "Repositories/StateRepository.h"
#include "Services/BLEHandler.h"
#include "Services/BMA456.h"
#include "Services/GPIOHandler.h"
#include "Services/NTPHandler.h"
#include "Services/PCF8563.h"
#include "Services/WifiHandler.h"
#include "View/ConfigPortalView.h"
#include "View/StepView.h"
#include "View/WatchFace.h"
#include "View/WatchyDisplay.h"
#include "ViewStates/ViewState.h"
#include "ViewStates/ViewStateFactory.h"
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

    PCF8563& rtc;
    GPIOHandler& gpio;
    StateRepository& stateRepo;

    void setViewState(VIEW_STATE_UID newStateID);

    // tracks if new view will be displayed
    bool m_viewChanged = false;
    bool m_busy = false;
    std::unique_ptr<ViewState> m_currentViewState;

protected:
public:
    // Get singleton instance of Controller class
    static Controller&
    getInstance();

    void handleWakeup();
    void handleButtons();
    void handleRadio();
    void handleBLE();
    void updateScreen();
    void sleep();
    bool isBusy();

    // remove copy constructor and assignment operator
    Controller(const Controller&) = delete;
    void operator=(const Controller&) = delete;
};