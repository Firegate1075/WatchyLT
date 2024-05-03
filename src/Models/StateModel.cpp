#include "StateModel.h"

bool StateModel::getInitialBoot() const
{
    return initialBoot;
}

VIEW_STATE StateModel::getViewState() const
{
    return viewState;
}

StateModel& StateModel::setInitialBoot(bool initialBoot)
{
    this->initialBoot = initialBoot;
    return *this;
}

StateModel& StateModel::setViewState(VIEW_STATE viewState)
{
    this->viewState = viewState;
    return *this;
}
