#pragma once

#include "Services/BMA456.h"
#include "Services/GPIOHandler.h"
#include "Services/PCF8563.h"
#include "View/WatchFace.h"
#include "ViewStates/ViewState.h"

class WatchFaceState : public ViewState {
public:
    WatchFaceState();
    ~WatchFaceState() = default;

    VIEW_STATE_UID handleButtons(uint8_t buttons);
    void updateScreen(bool doPartial);

    void onEnter();
    void onExit();

protected:
    PCF8563& rtc;
    GPIOHandler& gpio;
    WatchFace m_view;
};