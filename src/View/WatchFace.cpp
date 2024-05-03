#include "WatchFace.h"

RTC_DATA_ATTR u_int64_t counter = 0;

WatchFace::WatchFace()
    : screen(WatchyDisplay::getDisplay())
{
    Serial.println("WatchFace Constructor");
    Serial.flush();
}

void WatchFace::display()
{
    // rtc.getTimeDate(td);

    int16_t x1, y1, curX, curY;
    uint16_t w, h;

    screen.setFont(&Metropolis_Black11pt7b);

    screen.getTextBounds(String(counter), 0, 0, &x1, &y1, &w, &h);
    screen.setCursor(x1 + 4, -y1 + 4);
    screen.println(String(counter).c_str());

    curX = screen.getCursorX();
    curY = screen.getCursorY();

    screen.setFont(&Metropolis_Black16pt7b);
    // String timeStr = String(td.Hour) + ":" + String(td.Minute) + ":" + String(td.Second);
    // screen.getTextBounds(timeStr, curX, curY, &x1, &y1, &w, &h);
    screen.setCursor((CONST_DISPLAY::WIDTH - w) / 2, curY);
    // screen.println(timeStr.c_str());

    screen.setFont(&Metropolis_Black11pt7b);
    // screen.print(gpio.getBatteryVoltage());
    screen.println("V");
}
