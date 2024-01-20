#include "Controller.h"
#include "Repositories/CredentialRepository.h"
#include <Arduino.h>
void setup()
{
    CredentialRepository& credentialRepository = CredentialRepository::getInstace();
    // create Controller instance
    Controller& controller = Controller::getInstance();
}

void loop()
{
}
