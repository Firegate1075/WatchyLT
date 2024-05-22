#include "View.h"

View::View()
    : screen(WatchyDisplay::getDisplay())
{
}

void View::hibernate()
{
    screen.hibernate();
}