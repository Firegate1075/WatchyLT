#include "BLEHandler.h"

BLEHandler::BLEHandler()
{
}

void BLEHandler::initialize()
{
    //initialize the BLE environment
    BLEDevice::init("WatchyBluetoothLE");

    //create the server
    bleServer = BLEDevice::createServer();

    //create the service
    bleService = bleServer->createService("testservice");

    //create the characteristics
    bleCharacteristic = bleService->createCharacteristic("testcharacteristic", 0);

    //set the characteristic value
    bleCharacteristic->setValue("Hello World!");

    //start the service
    bleService->start();
}
