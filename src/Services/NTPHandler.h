#pragma once
#include "Arduino.h"
#include "ArduinoJson.h"
#include "HTTPClient.h"
#include "WifiObserver.h"
#include "constants.h"
#include "etl/string.h"

class NTPHandler : WifiObserver {
public:
    virtual void wifiCallback();

    etl::string<CONST_NETWORK::IP_LENGTH> getIP();
    pcfTime getTime();

protected:
private:
    HTTPClient http;
};