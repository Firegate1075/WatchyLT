#pragma once

#include <Arduino.h>

class CredentialModel {
private:
    String ssid;
    String password;

protected:
public:
    String getSSID();
    String getPassword();

    void setSSID(String ssid);
    void setPassword(String password);
};
