#include "StepView.h"

StepView::StepView()
{
}

// TODO: replace String with static memory (etl string)
void StepView::display(uint16_t steps, bool doPartial)
{
    int16_t x1, y1, curX, curY;
    uint16_t w, h;

    screen.setFont(&Metropolis_Black16pt7b);

    char stepStr[6];
    makeStepString(stepStr, steps);

    // print time centered
    screen.getTextBounds(stepStr, 0, 0, &x1, &y1, &w, &h);
    screen.setCursor((CONST_DISPLAY::WIDTH - w) / 2, (CONST_DISPLAY::HEIGHT + h) / 2);
    screen.println(stepStr);

    screen.display(doPartial);
}

void StepView::makeStepString(char* const str, uint16_t steps)
{
    str[0] = '0' + steps / 10000;
    str[1] = '0' + (steps % 10000) / 1000;
    str[2] = '0' + (steps % 1000) / 100;
    str[3] = '0' + (steps % 100) / 10;
    str[4] = '0' + (steps % 10);
    str[5] = '\0';
}