#pragma once

#include "DebugUtils.h"
#include "Metropolis_Font.h"
#include "View.h"
#include "constants.h"
#include <etl/string.h>
#include <etl/to_string.h>

class WatchFace : public View {
private:
    static void makeTimeString(char* const str, uint8_t h, uint8_t m);
    static etl::string<25> makeDateString(uint8_t day, uint8_t month, uint16_t year);

public:
    WatchFace();
    void display(const pcfTime& td, double vbat, bool doPartial);
};