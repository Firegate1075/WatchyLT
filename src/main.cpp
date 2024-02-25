#include "Controller.h"
#include <Arduino.h>

#include "Services/PCF8563.h"

void setup()
{
    // create Controller instance
    Serial.begin(115200);
    Controller& controller = Controller::getInstance();
}

void loop()
{
}
