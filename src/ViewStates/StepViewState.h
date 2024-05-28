#pragma once

#include "Services/BMA456.h"
#include "View/StepView.h"
#include "ViewStates/ViewState.h"

class StepViewState : public ViewState {
public:
    StepViewState();
    ~StepViewState() = default;

    VIEW_STATE_UID handleButtons(uint8_t buttons);
    void updateScreen(bool doPartial);

    void onEnter();
    void onExit();

protected:
    BMA456& bma456;
};