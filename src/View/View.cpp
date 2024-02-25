#include "View.h"
#include "Menus/MenuComponent.h"
#include "WatchFace.h"

View::View()
    : screen(WatchyDisplay::getDisplay())
{
    Serial.println("View constructor");
    Serial.flush();
    //WatchFace* watchFace = new WatchFace();
    //currentEntry = watchFace;
    Serial.println("View constructor END");
    Serial.flush();
}

void View::init(bool initialBoot)
{
    screen.init(0, initialBoot, 10, true);
    screen.setFullWindow();
    if (initialBoot) {
        screen.fillScreen(GxEPD_BLACK);
        screen.setTextColor(GxEPD_WHITE);
    }
}

void View::updateDisplay()
{
    currentEntry->display();
}

void View::endScreen()
{
    screen.hibernate();
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
