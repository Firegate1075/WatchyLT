#pragma once

#include "Menu.h"
#include "WiFiMenu.h"

class MainMenu : public Menu {
private:
    MainMenu();

    static const char* const menuEntryNames[];
    enum state {
        WiFi_MENU_SELECTED,
        ENTRY2_SELECTED,
        WiFi_MENU_INSIDE,
        ENTRY2_INSIDE
    };
    state currentState;

    WiFiMenu& wifiMenu = WiFiMenu::getInstance();
    GxEPD2_BW<WatchyDisplay, WatchyDisplay::HEIGHT> display = WatchyDisplay::getDisplay();

public:
    // Get singleton instance View class
    static MainMenu& getInstance()
    {
        static MainMenu instance;
        return instance;
    }
    // remove copy constructor and assignment operator
    MainMenu(const MainMenu&) = delete;
    void operator=(const MainMenu&) = delete;
    ~MainMenu() { }

    void displayMenu();
    bool handleBackButton();
    void handleEnterButton();
    void handleUpButton();
    void handleDownButton();
};