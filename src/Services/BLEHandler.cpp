#include "BLEHandler.h"

BLEHandler::BLEHandler()
{
}

//must be called first
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

    //set initial values of flags
    isAdvertising = false;
}

void BLEHandler::startAdvertising()
{
    //get advertising from ble server
    bleAdvertising = bleServer->getAdvertising();

    //start advertising
    if(isAdvertising){
        bleAdvertising->start();
        isAdvertising = true;
    }
}
