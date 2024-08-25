#include "CredentialRepository.h"
#include "iPreferences.h"

RTC_DATA_ATTR char ssidRaw[MAX_CREDENTIALS][32];
RTC_DATA_ATTR char passwordRaw[MAX_CREDENTIALS][64];

CredentialRepository::CredentialRepository()
{
    loadModelVector();
}

void CredentialRepository::saveModelVector()
{
    Preferences preferences;
    preferences.begin(CONST_PREFERENCES::CREDENTIALS_NAMESPACE, false);

    // remove stored credentials
    preferences.clear();

    for (const CredentialModel& model : modelVector) { // uint8_t index = 0; index < modelVector.size(); index++) {
        preferences.putString(model.getSSID().c_str(), model.getPassword().c_str());
    }

    preferences.end();
}

void CredentialRepository::loadModelVector()
{
    CredentialModel nextCredentials;

    iPreferences preferences;
    preferences.begin(CONST_PREFERENCES::CREDENTIALS_NAMESPACE, true, NULL);

    modelVector.clear();

    for (const auto& key : preferences.strings()) {
        if (modelVector.full()) {
            debugPrintln("too many credentials stored!");
        }

        nextCredentials.setSSID(key);
        nextCredentials.setPassword(preferences.getString(key).c_str());
        modelVector.push_back(nextCredentials);
    }

    preferences.end();
}

const etl::vector<CredentialModel, MAX_CREDENTIALS>& CredentialRepository::loadAll()
{
    return modelVector;
}

CredentialRepository& CredentialRepository::getInstance()
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
