#include "WatchFaceState.h"

WatchFaceState::WatchFaceState()
    : rtc(PCF8563::getInstance())
    , gpio(GPIOHandler::getInstance())
{
}

VIEW_STATE_UID WatchFaceState::handleButtons(uint8_t buttons)
{
    VIEW_STATE_UID nextState = VIEW_STATE_UID::WATCHFACE;

    if (buttons & CONST_BUTTON::UP) {
        nextState = VIEW_STATE_UID::STEP;
    }
    if (buttons & CONST_BUTTON::DOWN) {
        nextState = VIEW_STATE_UID::STEP;
    }

    return nextState;
}

void WatchFaceState::updateScreen(bool doPartial)
{
    WatchFace watchFace;
    pcfTime time;
    rtc.getTimeDate(time);
    double vbat = gpio.getBatteryVoltage();

    watchFace.display(time, vbat, doPartial);
}
