#pragma once

#include "constants.h"
#include <Models/StateModel.h>
#include <esp32-hal.h>

class StateRepository {
private:
    StateRepository();
    ~StateRepository() = default;

    StateModel model;
    void saveModel();
    void loadModel();

protected:
public:
    StateRepository(const StateRepository&) = delete;
    void operator=(const StateRepository&) = delete;
    static StateRepository& getInstance();

    const StateModel& load();
    void save(StateModel& stateModel);
};