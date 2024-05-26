#pragma once

#include "constants.h"
#include "etl/string.h"
#include <Arduino.h>

using etl::string;

class StateModel {
private:
    bool initialBoot;
    bool inMotion;
    VIEW_STATE viewState;

protected:
public:
    bool getInitialBoot() const;
    bool isInMotion() const;
    VIEW_STATE getViewState() const;

    StateModel& setInitialBoot(bool initialBoot);
    StateModel& setInMotion(bool inMotion);
    StateModel& setViewState(VIEW_STATE viewState);
};
