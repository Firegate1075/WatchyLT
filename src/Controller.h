#pragma once
#include <Arduino.h>
#include <esp32-hal.h>

/// @brief Controller class for Watchy
class Controller {
private:
    // private constructor and destructor
    Controller();
    ~Controller() { }

protected:
public:
    // Get singleton instance of Controller class
    static Controller& getInstance()
    {
        static Controller instance;
        return instance;
    }
    // remove copy constructor and assignment operator
    Controller(const Controller&) = delete;
    void operator=(const Controller&) = delete;
};