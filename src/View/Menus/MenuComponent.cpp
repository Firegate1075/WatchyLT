#include "MenuComponent.h"
#include "SubMenu.h"

// nullptr is intended to handle main menu / watchface. This does not work! TODO

void MenuComponent::handleBackButton()
{
    if (parent != nullptr) {
        view.setCurrentMenuEntry(parent);
    }
}

void MenuComponent::handleUpButton()
{
    if (parent != nullptr) {
        view.setCurrentMenuEntry(parent->getNextItem());
    }
}

void MenuComponent::handleDownButton()
{
    if (parent != nullptr) {
        view.setCurrentMenuEntry(parent->getPreviousItem());
    }
}
