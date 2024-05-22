#pragma once

#include "WatchyDisplay.h"
#include "constants.h"

class View {
private:
protected:
    GxEPD2_BW<WatchyDisplay, WatchyDisplay::HEIGHT>& screen;
    View();

public:
    void hibernate();
};