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
    static bool previousButton1 = 0;
    static bool previousButton2 = 0;
    static bool previousButton3 = 0;
    static bool previousButton4 = 0;

    bool button1 = digitalRead(BUTTON1);
    bool button2 = digitalRead(BUTTON2);
    bool button3 = digitalRead(BUTTON3);
    bool button4 = digitalRead(BUTTON4);

    uint8_t newMask;
    newMask = 0;
    // get transition
    newMask |= (!previousButton1 && button1) << 0;
    newMask |= (!previousButton2 && button2) << 1;
    newMask |= (!previousButton3 && button3) << 2;
    newMask |= (!previousButton4 && button4) << 3;

    previousButton1 = button1;
    previousButton2 = button2;
    previousButton3 = button3;
    previousButton4 = button4;

    m_mask = newMask;

    return newMask;
}
