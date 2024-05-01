#include "WatchFace.h"

RTC_DATA_ATTR u_int64_t counter = 0;

WatchFace::WatchFace()
    : screen(WatchyDisplay::getDisplay())
    , rtc(PCF8563::getInstance())
    , gpio(GPIOHandler::getInstance())
{
    Serial.println("WatchFace Constructor");
    Serial.flush();
}

MenuComponent* WatchFace::getNextItem()
{
    childrenIterator++;
    if (childrenIterator == children.end()) {
        childrenIterator = children.begin(); // Wrap around at end of vector
    }

    return *childrenIterator;
}

MenuComponent* WatchFace::getPreviousItem()
{
    if (childrenIterator == children.begin()) {
        childrenIterator = children.end(); // Wrap around after beginning of vector
    }
    childrenIterator--;
    return *childrenIterator;
}

void WatchFace::handleEnterButton()
{
    view.setCurrentMenuEntry(*childrenIterator);
}

void WatchFace::display()
{
    pcfTime td;
    rtc.getTimeDate(td);

    int16_t x1, y1, curX, curY;
    uint16_t w, h;

    screen.setFont(&Metropolis_Black11pt7b);

    screen.getTextBounds(String(counter), 0, 0, &x1, &y1, &w, &h);
    screen.setCursor(x1 + 4, -y1 + 4);
    screen.println(String(counter).c_str());

    curX = screen.getCursorX();
    curY = screen.getCursorY();

    screen.setFont(&Metropolis_Black16pt7b);
    String timeStr = String(td.Hour) + ":" + String(td.Minute) + ":" + String(td.Second);
    screen.getTextBounds(timeStr, curX, curY, &x1, &y1, &w, &h);
    screen.setCursor((CONST_DISPLAY::WIDTH - w) / 2, curY);
    screen.println(timeStr.c_str());

    screen.setFont(&Metropolis_Black11pt7b);
    screen.print(gpio.getBatteryVoltage());
    screen.println("V");

    screen.display(false);
}
