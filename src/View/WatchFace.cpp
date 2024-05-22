#include "WatchFace.h"

WatchFace::WatchFace()
{
    Serial.println("WatchFace Constructor");
    Serial.flush();
}

// TODO: replace String with static memory (etl string)
void WatchFace::display(const pcfTime& td, double vbat, bool doPartial)
{
    int16_t x1, y1, curX, curY;
    uint16_t w, h;

    screen.setFont(&Metropolis_Black16pt7b);

    char timeStr[6];
    makeTimeString(timeStr, td.Hour, td.Minute);

    // print time centered
    screen.getTextBounds(timeStr, 0, 0, &x1, &y1, &w, &h);
    screen.setCursor((CONST_DISPLAY::WIDTH - w) / 2, (CONST_DISPLAY::HEIGHT + h) / 2);
    screen.println(timeStr);

    screen.setFont(&Metropolis_Black11pt7b);
    screen.print(vbat);
    screen.println("V");

    screen.display(doPartial);
}

void WatchFace::makeTimeString(char* const str, uint8_t h, uint8_t m)
{
    str[0] = '0' + h / 10;
    str[1] = '0' + h % 10;
    str[2] = ':';
    str[3] = '0' + m / 10;
    str[4] = '0' + m % 10;
    str[5] = '\0';
}