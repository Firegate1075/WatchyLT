#pragma once

#include <Models/CredentialModel.h>
#include <esp32-hal.h>
#include <vector>

class CredentialRepository {
private:
    CredentialRepository();
    ~CredentialRepository() = default;

    std::vector<CredentialModel> modelVector;
    void saveModelVector();
    void loadModelVector();

protected:
public:
    CredentialRepository(const CredentialRepository&) = delete;
    void operator=(const CredentialRepository&) = delete;
    static CredentialRepository& getInstace();

    const std::vector<CredentialModel>& loadAll();
    bool save(CredentialModel&);
};