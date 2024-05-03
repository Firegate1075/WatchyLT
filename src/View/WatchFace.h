#pragma once

#include "Metropolis_Font.h"
#include "WatchyDisplay.h"
#include <etl/vector.h>

using CONST_MENU::MAX_MENU_SIZE;

class WatchFace {
private:
    GxEPD2_BW<WatchyDisplay, (uint16_t)200U>& screen;

public:
    WatchFace();
    void display();
};