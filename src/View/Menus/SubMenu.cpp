#include "SubMenu.h"

MenuComponent* SubMenu::getNextItem()
{
    childrenIterator++;
    if (childrenIterator == children.end()) {
        childrenIterator = children.begin(); // Wrap around at end of vector
    }

    return *childrenIterator;
}

MenuComponent* SubMenu::getPreviousItem()
{
    if (childrenIterator == children.begin()) {
        childrenIterator = children.end(); // Wrap around after beginning of vector
    }
    childrenIterator--;
    return *childrenIterator;
}

void SubMenu::handleEnterButton()
{
    view.setCurrentMenuEntry(*childrenIterator);
}

void SubMenu::display()
{
}
