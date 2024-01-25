#pragma once

#include "MenuComponent.h"
#include <etl/vector.h>

class SubMenu : public MenuComponent {
private:
    static const size_t MAX_MENU_SIZE = 8;

    etl::vector<MenuComponent*, MAX_MENU_SIZE> children;

public:
    MenuComponent* getNextItem();
    MenuComponent* getPreviousItem();

    virtual void display();
};