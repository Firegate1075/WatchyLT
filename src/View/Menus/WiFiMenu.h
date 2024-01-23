#include "Menu.h"

class WiFiMenu : public Menu {
private:
    WiFiMenu();

    static const char* const menuEntries[];
    enum state {
        BASE_MENU,
        ENTRY1_SELECTED,
        ENTRY2_SELECTED,
        ENTRY1_INSIDE,
        ENTRY2_INSIDE
    };
    state currentState;

public:
    // Get singleton instance View class
    static WiFiMenu& getInstance()
    {
        static WiFiMenu instance;
        return instance;
    }
    // remove copy constructor and assignment operator
    WiFiMenu(const WiFiMenu&) = delete;
    void operator=(const WiFiMenu&) = delete;
    ~WiFiMenu() { }

    void displayMenu();
    bool handleBackButton();
    void handleEnterButton();
    void handleUpButton();
    void handleDownButton();
};