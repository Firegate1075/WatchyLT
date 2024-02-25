#include "GPIOHandler.h"

using namespace CONST_PIN;

GPIOHandler::GPIOHandler() {
    analogReadResolution(12);
}

double GPIOHandler::getBatteryVoltage() {
    return analogReadMilliVolts(BATT_ADC) / 500.0;
}