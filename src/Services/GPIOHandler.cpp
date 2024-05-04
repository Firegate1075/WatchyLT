#include "GPIOHandler.h"

using CONST_PIN::BATT_ADC;

GPIOHandler::GPIOHandler()
{
    analogReadResolution(12);
}

double GPIOHandler::getBatteryVoltage()
{
    return analogReadMilliVolts(BATT_ADC) / 500.0;
}