#include "MainMenu.h"

const char* const MainMenu::menuEntryNames[] = { "Entry1", "Entry2" };

MainMenu::MainMenu()
    : currentState(WiFi_MENU_SELECTED)
{
}

void MainMenu::displayMenu()
{
}

bool MainMenu::handleBackButton()
{
    switch (currentState) {
    case WiFi_MENU_SELECTED:
        return false;
        break;

    case WiFi_MENU_INSIDE:
        if (wifiMenu.handleBackButton() == false) {
            currentState = WiFi_MENU_SELECTED;
            displayMenu();
        }
        break;

    default:
        return false;
        break;
    }
    return true;
}

void MainMenu::handleEnterButton()
{
    switch (currentState) {
    case WiFi_MENU_SELECTED:
        currentState = WiFi_MENU_INSIDE;
        wifiMenu.displayMenu();
        break;
    case WiFi_MENU_INSIDE:
        wifiMenu.handleEnterButton();
        break;
    default:
        break;
    }
}

void MainMenu::handleUpButton()
{
    switch (currentState) {
    case WiFi_MENU_SELECTED:
        currentState = ENTRY2_SELECTED;
        // TODO Display correct selction
        break;
    case WiFi_MENU_INSIDE:
        wifiMenu.handleUpButton();
        break;
    default:
        break;
    }
}

void MainMenu::handleDownButton()
{
    switch (currentState) {
    case WiFi_MENU_SELECTED:
        currentState = ENTRY2_SELECTED;
        // TODO Display correct selction
        break;
    case WiFi_MENU_INSIDE:
        wifiMenu.handleDownButton();
        break;
    default:
        break;
    }
}