#include "StateModel.h"

bool StateModel::getInitialBoot() const
{
    return initialBoot;
}

bool StateModel::isInMotion() const
{
    return inMotion;
}

VIEW_STATE_UID StateModel::getViewStateID() const
{
    return viewState;
}

StateModel& StateModel::setInitialBoot(bool initialBoot)
{
    this->initialBoot = initialBoot;
    return *this;
}

StateModel& StateModel::setInMotion(bool inMotion)
{
    this->inMotion = inMotion;
    return *this;
}

StateModel& StateModel::setViewState(VIEW_STATE_UID viewState)
{
    this->viewState = viewState;
    return *this;
}
