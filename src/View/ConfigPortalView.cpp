#include "ConfigPortalView.h"

ConfigPortalView::ConfigPortalView()
{
}

void ConfigPortalView::display(bool doPartial)
{
    int16_t x1, y1, curX, curY;
    uint16_t w, h;

    screen.setFont(&Metropolis_Black11pt7b);

    const char message1[] = "Hello There!";
    const char message2[] = "Configure the";
    const char message3[] = "WiFi or OTA";
    const char message4[] = "updates here:";
    String message5 = String("http://") + String(CONST_NETWORK::AP_IP_ADDRESS);

    const char message10[] = "<- Cancel";

    screen.getTextBounds(message1, 0, 0, &x1, &y1, &w, &h);
    screen.setCursor((CONST_DISPLAY::WIDTH - w) / 2, h + 16);
    screen.println(message1);

    screen.getTextBounds(message2, 0, 0, &x1, &y1, &w, &h);
    h = screen.getCursorY();
    screen.setCursor((CONST_DISPLAY::WIDTH - w) / 2, h + 10);
    screen.println(message2);

    screen.getTextBounds(message3, 0, 0, &x1, &y1, &w, &h);
    h = screen.getCursorY();
    screen.setCursor((CONST_DISPLAY::WIDTH - w) / 2, h);
    screen.println(message3);

    screen.getTextBounds(message4, 0, 0, &x1, &y1, &w, &h);
    h = screen.getCursorY();
    screen.setCursor((CONST_DISPLAY::WIDTH - w) / 2, h);
    screen.println(message4);

    screen.setFont(&Metropolis_Black10pt7b);
    screen.getTextBounds(message5, 0, 0, &x1, &y1, &w, &h);
    h = screen.getCursorY();
    screen.setCursor((CONST_DISPLAY::WIDTH - w) / 2, h + 8);
    screen.println(message5);

    screen.setFont(&Metropolis_Black11pt7b);
    screen.getTextBounds(message10, 0, 0, &x1, &y1, &w, &h);
    screen.setCursor(0, 190);
    screen.println(message10);

    screen.display(doPartial);
}
