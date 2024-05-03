#pragma once

#include "constants.h"
#include "etl/string.h"
#include <Arduino.h>

using etl::string;

class StateModel {
private:
    bool initialBoot;
    VIEW_STATE viewState;

protected:
public:
    bool getInitialBoot() const;
    VIEW_STATE getViewState() const;

    StateModel& setInitialBoot(bool initialBoot);
    StateModel& setViewState(VIEW_STATE viewState);
};
