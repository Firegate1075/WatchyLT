#include "ViewStateFactory.h"
// include ViewStates here to limit scope
#include "ViewStates/ConfigPortalViewState.h"
#include "ViewStates/StepViewState.h"
#include "ViewStates/WatchFaceState.h"

std::unique_ptr<ViewState> ViewStateFactory::createFromUID(VIEW_STATE_UID id)
{
    using std::unique_ptr;
    ViewState* newState; // use new here, since c++11 does not have make_unique

    switch (id) {
    case VIEW_STATE_UID::CONFIG_PORTAL:
        newState = new ConfigPortalViewState;
        break;
    case VIEW_STATE_UID::STEP:
        newState = new StepViewState;
        break;
    case VIEW_STATE_UID::WATCHFACE:
        newState = new WatchFaceState;
        break;

    default:
        // error -> return watchface
        newState = new WatchFaceState;
        break;
    }

    return std::unique_ptr<ViewState>(newState); // make unique
}
