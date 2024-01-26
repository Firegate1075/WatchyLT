#include "PCF8563.h"

PCF8563::PCF8563()
{
}

/**
 * @brief initialize PCF8563 sensor after reset
 *
 */
void PCF8563::init()
{
    rtc.initClock();
}
