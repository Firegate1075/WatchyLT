#pragma once

#include "WatchyDisplay.h"
#include "constants.h"

class View {
private:
protected:
    GxEPD2_BW<WatchyDisplay, WatchyDisplay::HEIGHT>& screen;
    View();

    void printlnHorizontallyCentered(const char* text, int16_t heightOffset = 0);
    void printlnAlignedRight(const char* text, int16_t heightOffset = 0);

public:
    void hibernate();
};