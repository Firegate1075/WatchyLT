#pragma once

#include "WatchyDisplay.h"
#include <memory>

class MenuComponent;

class View {
private:
    // private constructors
    View();
    ~View() { }
    MenuComponent* currentEntry;
    GxEPD2_BW<WatchyDisplay, (uint16_t)200U>& screen;

protected:
public:
    // Get singleton instance of View class
    static View& getInstance()
    {
        Serial.println("get view isntance");
        Serial.flush();
        static View instance;
        return instance;
    }
    // remove copy constructor and assignment operator
    View(const View&) = delete;
    void operator=(const View&) = delete;

    void init(bool initialBoot);
    void endScreen();

    void updateDisplay();
    void handleButtons();
    void setCurrentMenuEntry(MenuComponent* newEntry);
};