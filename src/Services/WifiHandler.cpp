#include "WifiHandler.h"

WifiHandler::WifiHandler()
    : wifiManager()
{
}

void WifiHandler::initialize()
{
    WiFi.mode(WIFI_STA);
    wifiManager.setConfigPortalBlocking(false);
    wifiManager.setConfigPortalTimeout(portalTimeout); // setTimeout does the same thing.
    wifiManager.setWiFiAutoReconnect(false);
    wifiManager.setConnectTimeout(connectTimeout);

    wifiManager.setAPCallback(onAPStart);
    wifiManager.setSaveConfigCallback(onConfigSaveAndConnect);
    wifiManager.setConfigPortalTimeoutCallback(onConfigPortalTimeout);
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
        credentials.setSSID(wifiManager.getWiFiSSID(false));
        credentials.setPassword(wifiManager.getWiFiPass(false));
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
 * @brief Connect to WiFi with Credentials. A connection failure will start the configuration portal This is always blocking.
 *
 * @param credentials The Credentials needed for the WiFi network
 * @return true -> Connection successfull
 * @return false -> Connection failed, configuration portal will be started
 */
bool WifiHandler::connectToNetwork(CredentialModel& credentials)
{
    bool success = wifiManager.autoConnect(credentials.getSSID().c_str(), credentials.getPassword().c_str());
    return success;
}

/**
 * @brief Disconnect from current network.
 *
 * @return true -> Disconnected successfully
 * @return false -> Disconnecting failed
 */
bool WifiHandler::disconnect()
{
    return wifiManager.disconnect();
}

/**
 * @brief Start configuration portal. Starting requiers loop() function call
 *
 * @return true -> start successfull, loop() function call is now needed
 * @return false -> start failed if portal is already running
 */
bool WifiHandler::openConfigurationPortal()
{
    return wifiManager.startConfigPortal(configurationPortalName);
}

/**
 * @brief Stop configuration portal
 *
 * @return true -> stopped successfully
 * @return false -> stopping failed
 */
bool WifiHandler::closeConfigurationPortal()
{
    return wifiManager.stopConfigPortal();
}

/**
 * @brief This needs to run in the main loop, if the WiFi Configuration AP is running.
 *
 */
void WifiHandler::loop()
{
    wifiManager.process();
}