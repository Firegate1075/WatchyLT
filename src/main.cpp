#include "Controller.h"
#include "Repositories/CredentialRepository.h"
#include <Arduino.h>

void setup()
{
    pin_config_t pin_configuration;
    pin_configuration.button1 = 26;
    pin_configuration.button2 = 25;
    pin_configuration.button3 = 35;
    pin_configuration.button4 = 4;
    pin_configuration.rtcInt = 27;
    pin_configuration.bmaInt1 = 14;
    pin_configuration.bmaInt2 = 12;

    // create Controller instance
    Controller& controller = Controller::createInstance(pin_configuration);
}

void loop()
{
}
