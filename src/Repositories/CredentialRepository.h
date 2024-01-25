#pragma once

#include "constants.h"
#include <Models/CredentialModel.h>
#include <esp32-hal.h>
#include <etl/vector.h>

using CONST_CREDENTIALS::MAX_CREDENTIALS;
using CONST_CREDENTIALS::PASS_LEN;
using CONST_CREDENTIALS::SSID_LEN;

class CredentialRepository {
private:
    CredentialRepository();
    ~CredentialRepository() = default;

    etl::vector<CredentialModel, MAX_CREDENTIALS> modelVector;
    void saveModelVector();
    void loadModelVector();

protected:
public:
    CredentialRepository(const CredentialRepository&) = delete;
    void operator=(const CredentialRepository&) = delete;
    static CredentialRepository& getInstace();

    const etl::vector<CredentialModel, MAX_CREDENTIALS>& loadAll();
    bool save(CredentialModel&);
};