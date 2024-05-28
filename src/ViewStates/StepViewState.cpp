#include "StepViewState.h"

StepViewState::StepViewState()
    : bma456(BMA456::getInstance())
{
}

VIEW_STATE_UID StepViewState::handleButtons(uint8_t buttons)
{
    VIEW_STATE_UID nextState = VIEW_STATE_UID::STEP; // assume no change

    if (buttons & CONST_BUTTON::BACK) {
        nextState = VIEW_STATE_UID::WATCHFACE;
    }
    if (buttons & CONST_BUTTON::UP) {
        nextState = VIEW_STATE_UID::WATCHFACE;
    }
    if (buttons & CONST_BUTTON::DOWN) {
        nextState = VIEW_STATE_UID::WATCHFACE;
    }
    return nextState;
}

void StepViewState::updateScreen(bool doPartial)
{
    StepView stepView;

    uint16_t steps = (uint16_t)bma456.getStepCounter();

    stepView.display(steps, doPartial);
}

void StepViewState::onEnter()
{
    bma456.setStepCountInterruptEnable(true);
}

void StepViewState::onExit()
{
    bma456.setStepCountInterruptEnable(false);
}
