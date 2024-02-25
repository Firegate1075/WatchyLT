#pragma once

#include "../Metropolis_Font.h"
#include "../WatchyDisplay.h"
#include "View/View.h"
#include "constants.h"
class SubMenu;

using CONST_MENU::MAX_MENU_SIZE;

class MenuComponent {
protected:
    SubMenu* parent;
    View& view = View::getInstance();

public:
    MenuComponent() { }
    virtual ~MenuComponent() { }
    virtual void display() = 0;

    /**
     * @brief Handle what happens when the back button (upper left) was pressed.
     *
     * @retval true -> The function handled the back button itslef (selfcontained action)
     * @retval false -> The function did nothing, calling function needs to handle exiting the menu
     */
    virtual void handleBackButton();

    /**
     * @brief Handle what happens when the enter button (lower left) was pressed.
     *
     */
    virtual void handleEnterButton() = 0;

    /**
     * @brief Handle what happens when the up button (upper right) was pressed.
     *
     */
    virtual void handleUpButton();
    /**
     * @brief Handle what happens when the up button (lower right) was pressed.
     *
     */
    virtual void handleDownButton();
};
