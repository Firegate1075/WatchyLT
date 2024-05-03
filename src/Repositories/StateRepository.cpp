#include "StateRepository.h"

RTC_DATA_ATTR bool initialBootRaw;
RTC_DATA_ATTR VIEW_STATE viewStateRaw;

StateRepository::StateRepository()
{
    loadModel();
}

void StateRepository::saveModel()
{
    initialBootRaw = model.getInitialBoot();
    viewStateRaw = model.getViewState();
}

void StateRepository::loadModel()
{
    model.setInitialBoot(initialBootRaw);
    model.setViewState(viewStateRaw);
}

const StateModel& StateRepository::load()
{
    return model;
}

StateRepository& StateRepository::getInstace()
{
    static StateRepository instance;
    return instance;
}

/// @brief Saves new StateModel in repository
/// @param model reference to StateModel
void StateRepository::save(StateModel& model)
{
    this->model = model;
    saveModel();
}
