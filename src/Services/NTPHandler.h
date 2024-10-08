#pragma once
#include "Arduino.h"
#include "ArduinoJson.h"
#include "DebugUtils.h"
#include "HTTPClient.h"
#include "WifiObserver.h"
#include "constants.h"
#include "etl/string.h"

class NTPHandler : WifiObserver {
public:
    virtual void wifiCallback();

    etl::string<CONST_NETWORK::IP_LENGTH> getIP();
    dateTime getTime();

protected:
private:
    HTTPClient http;
};