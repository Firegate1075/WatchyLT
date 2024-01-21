#include "CredentialRepository.h"

RTC_DATA_ATTR char ssidRaw[8][32];
RTC_DATA_ATTR char passwordRaw[8][64];

CredentialRepository::CredentialRepository()
    : modelVector(8)
{
    loadModelVector();
}

void CredentialRepository::saveModelVector()
{
    for (uint8_t index = 0; index < modelVector.size(); index++) {
        strcpy(ssidRaw[index], modelVector[index].getSSID().c_str());
        strcpy(passwordRaw[index], modelVector[index].getPassword().c_str());
    }
}

void CredentialRepository::loadModelVector()
{
    CredentialModel nextCredentials;

    modelVector.clear();
    for (uint8_t index = 0; index < modelVector.size(); index++) {
        nextCredentials.setSSID(String(ssidRaw[index]));
        nextCredentials.setPassword(String(passwordRaw[index]));
        modelVector.push_back(nextCredentials);
    }
}

const std::vector<CredentialModel>& CredentialRepository::loadAll()
{
    return modelVector;
}

CredentialRepository& CredentialRepository::getInstace()
{
    static CredentialRepository instance;
    return instance;
}

/// @brief Saves new CredentialModel in Repository
/// @param credentialModel reference to CredentialModel
/// @return returns true if success. Returns false if repository is full
bool CredentialRepository::save(CredentialModel& credentialModel)
{
    if (modelVector.size() >= 8)
        return false; // repo is full

    modelVector.push_back(credentialModel); // add new credentials
    saveModelVector();
    return true;
}
