#include "StateRepository.h"

RTC_DATA_ATTR bool initialBootRaw = true;
RTC_DATA_ATTR bool inMotionRaw = true;
RTC_DATA_ATTR VIEW_STATE viewStateRaw;

StateRepository::StateRepository()
{
    loadModel();
}

void StateRepository::saveModel()
{
    initialBootRaw = model.getInitialBoot();
    viewStateRaw = model.getViewState();
    inMotionRaw = model.isInMotion();
}

void StateRepository::loadModel()
{
    model.setInitialBoot(initialBootRaw);
    model.setViewState(viewStateRaw);
    model.setInMotion(inMotionRaw);
}

const StateModel& StateRepository::load() const
{
    return model;
}

StateRepository& StateRepository::getInstance()
{
    static StateRepository instance;
    return instance;
}

/// @brief Saves new StateModel in repository
/// @param model reference to StateModel
void StateRepository::save(const StateModel& model)
{
    this->model = model;
    saveModel();
}
