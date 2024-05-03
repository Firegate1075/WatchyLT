#include "CredentialModel.h"

string<SSID_LEN> CredentialModel::getSSID() const
{
    return ssid;
}

string<PASS_LEN> CredentialModel::getPassword() const
{
    return password;
}

void CredentialModel::setSSID(string<SSID_LEN> ssid)
{
    this->ssid = ssid;
}

void CredentialModel::setPassword(string<PASS_LEN> password)
{
    this->password = password;
}

void CredentialModel::setSSID(const char* ssid)
{
    this->ssid = string<SSID_LEN>(ssid);
}

void CredentialModel::setPassword(const char* password)
{
    this->password = string<PASS_LEN>(password);
}
