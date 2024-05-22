#pragma once

#include "Metropolis_Font.h"
#include "View.h"
#include "constants.h"
#include <etl/string.h>

class StepView : public View {
public:
    StepView();
    void display(uint16_t steps, bool doPartial);

private:
    void makeStepString(char* const str, uint16_t steps);
};