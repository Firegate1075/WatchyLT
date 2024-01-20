#include "CredentialModel.h"

String CredentialModel::getSSID()
{
    return ssid;
}

String CredentialModel::getPassword()
{
    return password;
}

void CredentialModel::setSSID(String ssid)
{
    this->ssid = ssid;
}

void CredentialModel::setPassword(String password)
{
    this->password = password;
}
