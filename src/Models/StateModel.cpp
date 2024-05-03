#include "StateModel.h"

bool StateModel::getInitialBoot()
{
    return initialBoot;
}

VIEW_STATE StateModel::getViewState()
{
    return viewState;
}

void StateModel::setInitialBoot(bool initialBoot)
{
    this->initialBoot = initialBoot;
}

void StateModel::setViewState(VIEW_STATE viewState)
{
    this->viewState = viewState;
}
