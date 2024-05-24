#include "ConfigPortalView.h"

ConfigPortalView::ConfigPortalView()
{
}

void ConfigPortalView::display(bool doPartial)
{
    int16_t x1, y1, curX, curY;
    uint16_t w, h;

    screen.setFont(&Metropolis_Black11pt7b);

    const char message[] = "Welcome to Watchy.\nConnect to Watchy's\nWifi AP!";

    // print time centered
    screen.getTextBounds(message, 0, 0, &x1, &y1, &w, &h);
    screen.setCursor((CONST_DISPLAY::WIDTH - w) / 2, (CONST_DISPLAY::HEIGHT - h) / 2);
    screen.println(message);

    screen.display(doPartial);
}
