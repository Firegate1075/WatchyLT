#include "Controller.h"
#include "Repositories/CredentialRepository.h"
#include <Arduino.h>
void setup()
{
    CredentialRepository credentialRepository();
    // create Controller instance
    Controller& controller = Controller::getInstance();
}

void loop()
{
}
