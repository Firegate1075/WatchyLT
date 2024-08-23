#include "Controller.h"
#include <Arduino.h>

void setup()
{
    // create Controller instance
    Serial.begin(115200);
    Controller& controller = Controller::getInstance();

    controller.handleWakeup();

    controller.handleButtons();
    controller.handleRadio();
    controller.updateScreen();
    while (controller.isBusy()) {
        controller.handleButtons();
        controller.handleRadio();
        // controller.handleBLE();
    }
    controller.updateScreen();

    controller.sleep();
}

void loop()
{
}
