#include "WatchFace.h"

RTC_DATA_ATTR u_int64_t counter = 0;

WatchFace::WatchFace()
    : screen(WatchyDisplay::getDisplay())
{
    Serial.println("WatchFace Constructor");
    Serial.flush();
}

// TODO: replace String with static memory (etl string)
void WatchFace::display(const pcfTime& td, double vbat, bool doPartial)
{
    int16_t x1, y1, curX, curY;
    uint16_t w, h;

    screen.setFont(&Metropolis_Black11pt7b);

    screen.getTextBounds(String(counter), 0, 0, &x1, &y1, &w, &h);
    screen.setCursor(x1 + 4, -y1 + 4);
    screen.println(String(counter).c_str());

    curX = screen.getCursorX();
    curY = screen.getCursorY();

    screen.setFont(&Metropolis_Black16pt7b);

    char timeStr[5];
    makeTimeString(timeStr, td.Hour, td.Minute);

    screen.getTextBounds(timeStr, curX, curY, &x1, &y1, &w, &h);
    screen.setCursor((CONST_DISPLAY::WIDTH - w) / 2, curY);
    screen.println(timeStr);

    screen.setFont(&Metropolis_Black11pt7b);
    screen.print(vbat);
    screen.println("V");

    screen.display(doPartial);
    screen.hibernate();
}

void WatchFace::makeTimeString(char* const str, uint8_t h, uint8_t m)
{
    str[0] = '0' + h / 10;
    str[1] = '0' + h % 10;
    str[2] = ':';
    str[3] = '0' + m / 10;
    str[4] = '0' + m % 10;
}