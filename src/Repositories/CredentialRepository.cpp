#include "CredentialRepository.h"

RTC_DATA_ATTR char ssidRaw[MAX_CREDENTIALS][32];
RTC_DATA_ATTR char passwordRaw[MAX_CREDENTIALS][64];

CredentialRepository::CredentialRepository()
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
    for (uint8_t index = 0; index < modelVector.max_size(); index++) {
        if (ssidRaw[index] == nullptr) {
            break; // got all saved credentials
        }

        nextCredentials.setSSID(string<SSID_LEN>(ssidRaw[index]));
        nextCredentials.setPassword(string<PASS_LEN>(passwordRaw[index]));
        modelVector.push_back(nextCredentials);
    }
}

const etl::vector<CredentialModel, MAX_CREDENTIALS>& CredentialRepository::loadAll()
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
    if (modelVector.full())
        return false; // repo is full

    modelVector.push_back(credentialModel); // add new credentials
    saveModelVector();
    return true;
}
