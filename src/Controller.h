#pragma once
#include <Arduino.h>
#include <esp32-hal.h>

/// @brief Controller class for Watchy
class Controller {
private:
    // private constructor and destructor
    Controller();
    ~Controller() { }

    void handleWakeup();

protected:
public:
    // Get singleton instance of Controller class
    static Controller& getInstance();

    // remove copy constructor and assignment operator
    Controller(const Controller&) = delete;
    void operator=(const Controller&) = delete;
};