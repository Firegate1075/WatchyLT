#include "ConfigPortalViewState.h"

ConfigPortalViewState::ConfigPortalViewState()
    : wifi(WifiHandler::getInstance())
{
}

VIEW_STATE_UID ConfigPortalViewState::handleButtons(uint8_t buttons)
{
    VIEW_STATE_UID nextState = VIEW_STATE_UID::CONFIG_PORTAL;

    if (buttons & CONST_BUTTON::BACK) {
        nextState = VIEW_STATE_UID::WATCHFACE;
        debugPrintln("leaving config portal");
    } else if (buttons & CONST_BUTTON::UP) {
        nextState = VIEW_STATE_UID::STEP;
    } else if (buttons & CONST_BUTTON::DOWN) {
        nextState = VIEW_STATE_UID::WATCHFACE;
    }

    return nextState;
}

void ConfigPortalViewState::updateScreen(bool doPartial)
{
    m_view.display(doPartial);
}

void ConfigPortalViewState::onEnter()
{
    wifi.openConfigurationPortal();
    m_busy = true; // dont sleep while open
}

void ConfigPortalViewState::onExit()
{
    CredentialRepository& credentialRepo = CredentialRepository::getInstance();

    CredentialModel newModel = wifi.getCredentialsOfCurrentNetwork();
    const etl::vector<CredentialModel, CONST_CREDENTIALS::MAX_CREDENTIALS>& vec = credentialRepo.loadAll();

    bool alreadySaved = false;
    for (CredentialModel model : vec) {
        if (model.getSSID() == newModel.getSSID()) {
            alreadySaved = true;
            break;
        }
    }

    if (!alreadySaved && newModel.getSSID().size() > 0) {
        credentialRepo.save(newModel);
    }

    if (wifi.isConfigurationPortalOpen()) {
        wifi.closeConfigurationPortal();
    }

    if (wifi.isConnected() || wifi.connectToNetwork(credentialRepo.loadAll())) {
        // sync time while connected
        NTPHandler ntp;
        dateTime currentTime = ntp.getTime();

        PCF8563& rtc = PCF8563::getInstance();
        rtc.setTimeDate(currentTime);
    }

    m_busy = false; // we may sleep now
}
