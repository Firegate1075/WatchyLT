#include "Controller.h"
#include <Arduino.h>

void setup()
{
    // create Controller instance
    Serial.begin(115200);
    Controller& controller = Controller::getInstance();

    controller.handleWakeup();
    controller.handleButtons();
    controller.updateScreen();
    controller.sleep();
}

void loop()
{
}
