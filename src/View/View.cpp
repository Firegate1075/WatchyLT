#include "View.h"

View::View()
    : screen(WatchyDisplay::getDisplay())
{
}

/**
 * @brief Print a line of text honrizintally centered on the screen. You need to set the correct font and height before calling this function.
 * Though a height offset can be set. The offset may be needed when changing the font between new lines.
 *
 * @param text Text to print.
 * @param heightAdjustment Use this to offset the height, if needed. Negative values will move the text up, positives down.
 */
void View::printlnHorizontallyCentered(const char* text, int16_t heightOffset)
{
    int16_t x1, y1, curX, curY;
    uint16_t w, h;

    curY = screen.getCursorY();

    screen.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
    screen.setCursor((CONST_DISPLAY::WIDTH - w) / 2, curY + heightOffset);

    screen.println(text);
}

/**
 * @brief Print a line of text aligned to the right on the screen. You need to set the correct font and height before calling this function.
 * Though a height offset can be set. The offset may be needed when changing the font between new lines.
 *
 * @param text Text to print.
 * @param heightAdjustment Use this to offset the height, if needed. Negative values will move the text up, positives down.
 */
void View::printlnAlignedRight(const char* text, int16_t heightOffset)
{
    int16_t x1, y1, curX, curY;
    uint16_t w, h;

    curY = screen.getCursorY();

    screen.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
    // Subtract 1, since the display width is 200 but the most right pixel is 199
    screen.setCursor((CONST_DISPLAY::WIDTH - w - 1), curY + heightOffset);

    screen.println(text);
}

void View::hibernate()
{
    screen.hibernate();
}