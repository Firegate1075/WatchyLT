#pragma once

#include "MenuComponent.h"
#include <etl/vector.h>

class SubMenu : public MenuComponent {
private:
    etl::vector<MenuComponent*, MAX_MENU_SIZE> children;
    etl::vector<MenuComponent*, MAX_MENU_SIZE>::iterator childrenIterator; // TODO: iterator needs to be initialized

public:
    MenuComponent* getNextItem();
    MenuComponent* getPreviousItem();

    virtual void handleEnterButton();
    virtual void display();
};