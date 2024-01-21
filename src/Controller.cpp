#include "Controller.h"

RTC_DATA_ATTR bool initialBoot = true;

Controller::Controller()
{
    if (initialBoot) // handle initialBoot (wakeup after flashing)
        ;
}