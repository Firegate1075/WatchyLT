#pragma once

#include "constants.h"

class ViewState {
public:
    virtual VIEW_STATE_UID handleButtons(uint8_t buttons) = 0;
    virtual void updateScreen(bool doPartial) = 0;

    bool isBusy() { return m_busy; }

    // hook interface for optional callbacks
    virtual void onEnter() { }
    virtual void onExit() { }

    virtual ~ViewState() = default;

protected:
    bool m_busy = false; // may be set by ViewStates, when waiting for wifi
};