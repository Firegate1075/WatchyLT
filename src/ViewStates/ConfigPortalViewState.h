#pragma once

#include "Repositories/CredentialRepository.h"
#include "Services/NTPHandler.h"
#include "Services/PCF8563.h"
#include "Services/WifiHandler.h"
#include "View/ConfigPortalView.h"
#include "ViewStates/ViewState.h"

class ConfigPortalViewState : public ViewState {
public:
    ConfigPortalViewState();
    ~ConfigPortalViewState() = default;

    VIEW_STATE_UID handleButtons(uint8_t buttons);
    void updateScreen(bool doPartial);

    void onEnter();
    void onExit();

protected:
    WifiHandler& wifi;
    ConfigPortalView m_view;
};