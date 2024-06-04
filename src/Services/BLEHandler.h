#pragma once
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEService.h>
#include <BLECharacteristic.h>
#include <BLEAdvertising.h>

class BLEHandler {
private:
    // private constructor and destructor
    BLEHandler();
    ~BLEHandler() { }

    BLEServer* bleServer;
    BLEService* bleService;
    BLECharacteristic* bleCharacteristic;
    BLEAdvertising* bleAdvertising;
    //flags
    bool isAdvertising;

protected:
public:
    // Get singleton instance of BLEHandler class
    static BLEHandler& getInstance()
    {
        static BLEHandler instance;
        return instance;
    }
    // remove copy constructor and assignment operator
    BLEHandler(const BLEHandler&) = delete;
    void operator=(const BLEHandler&) = delete;

    void initialize();
    void startAdvertising();
    bool isServerAdvertising();
};