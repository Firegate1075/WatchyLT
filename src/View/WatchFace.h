#pragma once

#include "Metropolis_Font.h"
#include "View.h"
#include "constants.h"
#include <etl/string.h>

class WatchFace : public View {
private:
    static void makeTimeString(char* const str, uint8_t h, uint8_t m);

public:
    WatchFace();
    void display(const pcfTime& td, double vbat, bool doPartial);
};