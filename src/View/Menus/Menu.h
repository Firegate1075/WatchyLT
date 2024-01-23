#pragma once

#include "../Metropolis_Font.h"
#include "../WatchyDisplay.h"

class Menu {
public:
    Menu() { }
    virtual ~Menu() { }
    virtual void displayMenu() = 0;

    /**
     * @brief Handle what happens when the back button (upper left) was pressed.
     *
     * @retval true -> The function handled the back button itslef (selfcontained action)
     * @retval false -> The function did nothing, calling function needs to handle exiting the menu
     */
    virtual bool handleBackButton() = 0;

    /**
     * @brief Handle what happens when the enter button (lower left) was pressed.
     *
     */
    virtual void handleEnterButton() = 0;

    /**
     * @brief Handle what happens when the up button (upper right) was pressed.
     *
     */
    virtual void handleUpButton() = 0;

    /**
     * @brief Handle what happens when the up button (lower right) was pressed.
     *
     */
    virtual void handleDownButton() = 0;
};