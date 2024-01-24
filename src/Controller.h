#pragma once
#include <Arduino.h>
#include <esp32-hal.h>

typedef struct {
    uint8_t button1;
    uint8_t button2;
    uint8_t button3;
    uint8_t button4;

    uint8_t rtcInt;

    uint8_t bmaInt1;
    uint8_t bmaInt2;
} pin_config_t;

/// @brief Controller class for Watchy
class Controller {
private:
    // private constructor and destructor
    Controller() = delete;
    Controller(pin_config_t pinConfiguration);
    ~Controller() { delete instancePointer; }

    // pointer to singleton instance
    static Controller* instancePointer;

    void handleWakeup();

    pin_config_t pinConfig;

protected:
public:
    // Get singleton instance of Controller class
    static Controller& getInstance();
    static Controller& createInstance(const pin_config_t pinConfiguration);

    // remove copy constructor and assignment operator
    Controller(const Controller&) = delete;
    void operator=(const Controller&) = delete;
};