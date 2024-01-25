#pragma once

#include "MenuComponent.h"

class MenuItem : public MenuComponent {
private:
protected:
public:
    virtual void handleEnterButton() = 0;
    virtual void display() = 0;
};