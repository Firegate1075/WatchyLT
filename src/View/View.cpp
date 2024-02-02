#include "View.h"
#include "Menus/MenuComponent.h"

View::View()
{
}

void View::updateDisplay()
{
    currentEntry->display();
}

void View::handleButtons()
{
    // TODO: implement handling for multiple button presses
    bool button1pressed = digitalRead(CONST_PIN::BUTTON1); // button1 = back
    bool button2pressed = digitalRead(CONST_PIN::BUTTON2); // button2 = enter
    bool button3pressed = digitalRead(CONST_PIN::BUTTON3); // button3 = up
    bool button4pressed = digitalRead(CONST_PIN::BUTTON4); // button4 = down

    if (button1pressed) {
        currentEntry->handleBackButton();
    }
    if (button2pressed) {
        currentEntry->handleEnterButton();
    }
    if (button3pressed) {
        currentEntry->handleUpButton();
    }
    if (button4pressed) {
        currentEntry->handleDownButton();
    }
}

void View::setCurrentMenuEntry(MenuComponent* newEntry)
{
    currentEntry = newEntry;
}
