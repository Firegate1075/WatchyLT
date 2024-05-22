#include "GPIOHandler.h"

using CONST_PIN::BATT_ADC;
using CONST_PIN::BUTTON1;
using CONST_PIN::BUTTON2;
using CONST_PIN::BUTTON3;
using CONST_PIN::BUTTON4;

GPIOHandler::GPIOHandler()
{
    analogReadResolution(12);

    pinMode(BUTTON1, INPUT);
    pinMode(BUTTON2, INPUT);
    pinMode(BUTTON3, INPUT);
    pinMode(BUTTON4, INPUT);
    // TODO: set all pin modes
}

double GPIOHandler::getBatteryVoltage()
{
    return analogReadMilliVolts(BATT_ADC) / 500.0;
}

/// @brief get mask of pressed buttons.
/// BUTTON1 <-> bit 0
/// ...
/// BUTTON4 <-> bit 3
/// @return byte, where 4 lsb represent pressed buttons
uint8_t GPIOHandler::getButtonMask()
{
    return m_mask;
}

uint8_t GPIOHandler::readButtons()
{
    uint8_t newMask;
    newMask = 0;
    // get transition
    newMask |= (!(m_mask & 1) && digitalRead(BUTTON1)) << 0;
    newMask |= (!(m_mask & 2) && digitalRead(BUTTON2)) << 1;
    newMask |= (!(m_mask & 4) && digitalRead(BUTTON3)) << 2;
    newMask |= (!(m_mask & 8) && digitalRead(BUTTON4)) << 3;

    m_mask = newMask;

    return newMask;
}
