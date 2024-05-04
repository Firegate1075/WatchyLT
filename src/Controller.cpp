#include "Controller.h"

Controller::Controller()
    : rtc(PCF8563::getInstance())
    , gpio(GPIOHandler::getInstance())
    , stateRepo(StateRepository::getInstance())
{
    Wire.begin(SDA, SCL);

    debugPrint("Initial boot: ");
    debugPrintln(stateRepo.load().getInitialBoot());

    // handle initialBoot (wakeup after flashing)
    if (stateRepo.load().getInitialBoot()) {
        // initialize sensors

        // BMA needs small delay to be setup correctly
        delay(1000);

        BMA456::getInstance().init();
        rtc.resetRTC();
        rtc.resetAlarm();
    }

    // TODO: set pin modes

    // configure wake up pins
    uint64_t wakeupPinMask = 0;
    wakeupPinMask |= ((uint64_t)1 << CONST_PIN::BUTTON1);
    wakeupPinMask |= ((uint64_t)1 << CONST_PIN::BUTTON2);
    wakeupPinMask |= ((uint64_t)1 << CONST_PIN::BUTTON3);
    wakeupPinMask |= ((uint64_t)1 << CONST_PIN::BUTTON4);
    wakeupPinMask |= ((uint64_t)1 << CONST_PIN::BMA_INT1);
    wakeupPinMask |= ((uint64_t)1 << CONST_PIN::BMA_INT2);

    handleWakeup();

    VIEW_STATE previousView = stateRepo.load().getViewState();
    VIEW_STATE nextView = handleScreen(previousView);
    while (nextView != previousView) { // update screen while view changed
        previousView = nextView;
        nextView = handleScreen(previousView); // TODO: screen.hibernate() is now called before displaying is finished
    }

    // wake up on RTC interrupt (active low)
    esp_sleep_enable_ext0_wakeup((gpio_num_t)CONST_PIN::RTC_INT, LOW);
    // wake up on any button press or BMA456 interrupt(active high)
    esp_sleep_enable_ext1_wakeup(wakeupPinMask, ESP_EXT1_WAKEUP_ANY_HIGH);

    StateModel stateModel = stateRepo.load();
    stateModel.setInitialBoot(false);
    stateRepo.save(stateModel);

    // configure sensors (or maybe only before use ?)
    esp_deep_sleep_start();
}

/**
 * @brief handler method for proccessing different wakeup reasons
 *
 */
void Controller::handleWakeup()
{
    // retrieve cause of wakeup
    esp_sleep_wakeup_cause_t cause = esp_sleep_get_wakeup_cause();
    BMA456_interrupt bma_interrupt = BMA456::getInstance().getInterrupt();

    switch (cause) {
    case ESP_SLEEP_WAKEUP_EXT0:
        debugPrintln("RTC Wakeup");
        rtc.resetAlarm();

        // RTC wakeup
        break;
    case ESP_SLEEP_WAKEUP_EXT1:
        if (bma_interrupt != BMA456_interrupt::NONE) {
            debugPrintln("BMA wakeup");
        } else {
            debugPrintln("Button wakeup");
            // viewObj.handleButtons();
        }
        // button interrupt or bma interrupt
        break;
    default:
        debugPrint("unexpected wakeup: ");
        debugPrintln(cause);

        // error: unexpected wakeup
        break;
    }
}

/// @brief handle screen displaying and user inputs
/// @param currentState `VIEW_STATE` tracks current view
/// @return `VIEW_STATE` next view
VIEW_STATE Controller::handleScreen(VIEW_STATE currentState)
{
    // initialize display (ugly here, maybe static method in WatchyDisplay? maybe have class inherit from GxEPD2_BW<WatchyDisplay, WatchyDisplay::HEIGHT>?)

    GxEPD2_BW<WatchyDisplay, WatchyDisplay::HEIGHT>& screen = WatchyDisplay::getDisplay();
    screen.init(0, stateRepo.load().getInitialBoot(), 10, true);
    screen.setFullWindow();

    if (stateRepo.load().getInitialBoot()) {
        screen.fillScreen(GxEPD_BLACK);
        screen.setTextColor(GxEPD_WHITE);
    }

    VIEW_STATE nextState = currentState;

    switch (currentState) {
    case VIEW_STATE::WATCHFACE: {
        WatchFace watchFace;
        pcfTime time;
        rtc.getTimeDate(time);
        double vbat = gpio.getBatteryVoltage();

        watchFace.display(time, vbat, !stateRepo.load().getInitialBoot());
        nextState = VIEW_STATE::WATCHFACE;
        // buttons
    } break;

    default:
        break;
    }

    // screen.hibernate() here?
    return nextState;
}

Controller& Controller::getInstance()
{
    static Controller instance;
    return instance;
}
