#include "BLEHandler.h"

BLEHandler::BLEHandler()
{
}

// must be called first
void BLEHandler::initialize()
{
    // initialize the BLE environment
    BLEDevice::init("WatchyBluetoothLE");

    // create the server
    bleServer = BLEDevice::createServer();

    // create the service
    bleService = bleServer->createService("18796165-5846-49a9-bb18-60a19b87b659");

    // create the characteristics
    bleCharacteristic = bleService->createCharacteristic("cb09c2d0-ff16-4e90-b1b3-dd7937477fa5", BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);

    // set the characteristic value
    bleCharacteristic->setValue("Hello World!");

    // start the service
    bleService->start();

    // set initial values of flags
    BLEHandler::isAdvertising = false;
}

void BLEHandler::startAdvertising()
{
    // get advertising from ble server
    bleAdvertising = bleServer->getAdvertising();

    // start advertising
    if (!isAdvertising) {
        bleAdvertising->addServiceUUID("18796165-5846-49a9-bb18-60a19b87b659");
        bleAdvertising->setScanResponse(true);
        bleAdvertising->setMinPreferred(0x06); // functions that help with iPhone connections issue
        bleAdvertising->setMinPreferred(0x12);

        bleAdvertising->start();
        isAdvertising = true;
    }
}

bool BLEHandler::isServerAdvertising()
{
    return isAdvertising;
}
