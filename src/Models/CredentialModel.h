#pragma once

#include "constants.h"
#include "etl/string.h"
#include <Arduino.h>

using CONST_CREDENTIALS::MAX_CREDENTIALS;
using CONST_CREDENTIALS::PASS_LEN;
using CONST_CREDENTIALS::SSID_LEN;

using etl::string;

class CredentialModel {
private:
    string<SSID_LEN> ssid;
    string<PASS_LEN> password;

protected:
public:
    string<SSID_LEN> getSSID();
    string<PASS_LEN> getPassword();

    void setSSID(string<SSID_LEN> ssid);
    void setPassword(string<PASS_LEN> password);
    void setSSID(const char* ssid);
    void setPassword(const char* password);
};
