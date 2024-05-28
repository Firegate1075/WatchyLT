#pragma once

#include "ViewStates/ViewState.h"
#include "constants.h"
#include <memory>

class ViewStateFactory {
public:
    static std::unique_ptr<ViewState> createFromUID(VIEW_STATE_UID id);
};
