#pragma once

#include "Models/CredentialModel.h"
#include <WifiManager.h>

class WifiHandler {
private:
    // private constructor and destructor
    WifiHandler();
    ~WifiHandler() { }

    static void onAPStart(WiFiManager* wifiManager);
    static void onConfigSaveAndConnect();
    static void onConfigPortalTimeout();

    WiFiManager wifiManager;

    const uint8_t portalTimeout = 60;
    const uint8_t connectTimeout = 30;
    const char* configurationPortalName = "Watchy-AP";

protected:
public:
    // Get singleton instance of WifiHandler class
    static WifiHandler& getInstance()
    {
        static WifiHandler instance;
        return instance;
    }
    // remove copy constructor and assignment operator
    WifiHandler(const WifiHandler&) = delete;
    void operator=(const WifiHandler&) = delete;

    void initialize();
    CredentialModel getCredentialsOfCurrentNetwork();
    bool connectToNetwork(CredentialModel& credentials);
    bool disconnect();
    bool openConfigurationPortal();
    bool closeConfigurationPortal();
    void loop();
};