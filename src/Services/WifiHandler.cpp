#include "WifiHandler.h"
#include "Arduino.h"
WifiHandler::WifiHandler()
    : wifiManager()
{
    initialize();
}

void WifiHandler::initialize()
{
    WiFi.mode(WIFI_STA);
    wifiManager.setConfigPortalBlocking(false);
    // wifiManager.setConfigPortalTimeout(CONST_WIFI::portalTimeout); // setTimeout does the same thing.
    wifiManager.setWiFiAutoReconnect(false);
    // wifiManager.setConnectTimeout(CONST_WIFI::connectTimeout);

    wifiManager.setAPCallback(onAPStart);
    wifiManager.setSaveConfigCallback(onConfigSaveAndConnect);
    wifiManager.setConfigPortalTimeoutCallback(onConfigPortalTimeout);
    Serial.println("initialized");
}

/**
 * @brief Retrieve SSID and password of currently connected network.
 *
 * @return CredentialModel -> Returns empty Strings if WiFi is not connected.
 */
CredentialModel WifiHandler::getCredentialsOfCurrentNetwork()
{
    CredentialModel credentials;
    if (WiFi.isConnected()) {
        credentials.setSSID(wifiManager.getWiFiSSID(false).c_str());
        credentials.setPassword(wifiManager.getWiFiPass(false).c_str());
    }
    return credentials;
}

/**
 * @brief This will be called, after the WiFiManger starts the Configuration AP. Instructions on how to add a new network will be displayed.
 *
 */
void WifiHandler::onAPStart(WiFiManager* wifiManager)
{
    // TODO Display instructions to connect and configure WiFi
}

/**
 * @brief This will be called after saving and connecting to a network.
 *
 */
void WifiHandler::onConfigSaveAndConnect()
{
    // TODO Notify Controller(?) new Credential need to be stored
}

/**
 * @brief This will be called after the Configuration Portal times out. It will display a message that the portal timed out.
 *
 */
void WifiHandler::onConfigPortalTimeout()
{
    // TODO Display timeout message
}

/**
 * @brief Notifies observers about established WiFi connection
 *
 */
void WifiHandler::observerCallback()
{
    for_each(observers.begin(), observers.end(), [](WifiObserver* observer) {
        if (observer)
            observer->wifiCallback();
    });
}

/**
 * @brief Connect to WiFi with Credentials. This is always blocking.
 *
 * @param credentials The Credentials needed for the WiFi network
 * @retval true -> Connection successfull
 * @retval false -> Connection failed
 */
bool WifiHandler::connectToNetwork(const vector<CredentialModel, CONST_CREDENTIALS::MAX_CREDENTIALS>& credentials)
{
    for (auto credential : credentials) {
        uint16_t networkCount = WiFi.scanNetworks();
        for (uint16_t networkIndex = 0; networkIndex < networkCount; networkIndex++) {
            if (credential.getSSID() == WiFi.SSID(networkIndex).c_str()) {
                return WiFi.begin(credential.getSSID().c_str(), credential.getPassword().c_str()) == WL_CONNECTED;
            }
        }
    }

    // no credentials available for networks
    return false;
}

/**
 * @brief Disconnect from current network.
 *
 * @retval true -> Disconnected successfully
 * @retval false -> Disconnecting failed
 */
bool WifiHandler::disconnect()
{
    return wifiManager.disconnect();
}

/**
 * @brief Start configuration portal. Starting requiers loop() function call
 *
 * @retval true -> start successfull, loop() function call is now needed
 * @retval false -> start failed if portal is already running
 */
bool WifiHandler::openConfigurationPortal()
{
    return wifiManager.startConfigPortal(CONST_WIFI::configurationPortalName);
}

/**
 * @brief Stop configuration portal
 *
 * @retval true -> stopped successfully
 * @retval false -> stopping failed
 */
bool WifiHandler::closeConfigurationPortal()
{
    return wifiManager.stopConfigPortal();
}

bool WifiHandler::isConfigurationPortalOpen()
{
    return wifiManager.getConfigPortalActive();
}

/**
 * @brief This needs to run in the main loop, if the WiFi Configuration AP is running.
 *
 */
void WifiHandler::loop()
{
    wifiManager.process();
}

/**
 * @brief Attach observer for WifiEstablished connection
 *
 * @param observer Pointer to observer object
 * @retval true -> success
 * @retval false -> fail
 */
bool WifiHandler::attach(WifiObserver* observer)
{
    if (observers.full()) {
        return false;
    }

    observers.push_back(observer);
    return true;
}

/**
 * @brief Detach observer
 *
 * @param observer
 */
void WifiHandler::detach(WifiObserver* observer)
{
    observers.remove(observer);
}
