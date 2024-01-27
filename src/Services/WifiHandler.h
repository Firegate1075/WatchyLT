#pragma once

#include "Models/CredentialModel.h"
#include "WifiObserver.h"
#include "constants.h"
#include "etl/list.h"
#include <WifiManager.h>

using etl::for_each;
using etl::list;

class WifiHandler {
private:
    // private constructor and destructor
    WifiHandler();
    ~WifiHandler() { }

    static void onAPStart(WiFiManager* wifiManager);
    static void onConfigSaveAndConnect();
    static void onConfigPortalTimeout();
    static void observerCallback();

    WiFiManager wifiManager;
    static list<WifiObserver*, CONST_WIFI::MAX_OBSERVERS> observers;
    static list<WifiObserver*, CONST_WIFI::MAX_OBSERVERS>::iterator observerIterator;

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
    bool attach(WifiObserver* observer);
    void detach(WifiObserver* observer);
};