#pragma once

#include "Menus/MenuComponent.h"
#include "Menus/MenuItem.h"
#include "Services/GPIOHandler.h"
#include "Services/PCF8563.h"
#include "WatchyDisplay.h"
#include <etl/vector.h>

using CONST_MENU::MAX_MENU_SIZE;

class WatchFace : public MenuComponent {
private:
    GxEPD2_BW<WatchyDisplay, (uint16_t)200U>& screen;
    PCF8563& rtc;
    GPIOHandler& gpio;

    etl::vector<MenuComponent*, MAX_MENU_SIZE> children;
    etl::vector<MenuComponent*, MAX_MENU_SIZE>::iterator childrenIterator; // TODO: iterator needs to be initialized

public:
    WatchFace();
    MenuComponent* getNextItem();
    MenuComponent* getPreviousItem();

    virtual void handleEnterButton();
    virtual void display();
};