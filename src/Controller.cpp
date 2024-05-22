#include "Controller.h"

Controller::Controller()
    : rtc(PCF8563::getInstance())
    , gpio(GPIOHandler::getInstance())
    , stateRepo(StateRepository::getInstance())
{
    Wire.begin(SDA, SCL);

    debugPrint("Initial boot=");
    debugPrintln(stateRepo.load().getInitialBoot());

    // handle initialBoot (wakeup after flashing)
    if (stateRepo.load().getInitialBoot()) {
        // initialize sensors

        // BMA needs small delay to be setup correctly
        delay(1000);

        BMA456& bma456 = BMA456::getInstance();

        bma456.init();
        bma456.enableStepCounter();
        bma456.setStepCounterWatermark(1);
        bma456.setStepCountInterruptEnable();

        rtc.resetRTC();
        rtc.resetAlarm();

        // set initalBoot false
        StateModel stateModel = stateRepo.load();
        stateModel.setInitialBoot(false);
        stateRepo.save(stateModel);
    }
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

/// @brief handle user inputs
void Controller::handleButtons()
{
    VIEW_STATE currentState = stateRepo.load().getViewState();
    VIEW_STATE nextState = currentState; // assume no change, unless specified

    uint8_t buttons = gpio.readButtons();
    using CONST_BUTTON::BACK;
    using CONST_BUTTON::DOWN;
    using CONST_BUTTON::MENU;
    using CONST_BUTTON::UP;

    switch (currentState) {
    case VIEW_STATE::WATCHFACE: {
        if (buttons & UP) {
            nextState = VIEW_STATE::STEP;
        }
        if (buttons & DOWN) {
            nextState = VIEW_STATE::STEP;
        }
    } break;
    case VIEW_STATE::STEP: {
        if (buttons & BACK) {
            nextState = VIEW_STATE::WATCHFACE;
        }
        if (buttons & UP) {
            nextState = VIEW_STATE::WATCHFACE;
        }
        if (buttons & DOWN) {
            nextState = VIEW_STATE::WATCHFACE;
        }

    } break;

    default:
        break;
    }

    if (nextState != currentState) {
        m_viewChanged = true;

        // save new view state
        StateModel stateModel = stateRepo.load();
        stateModel.setViewState(nextState);
        stateRepo.save(stateModel);
    } else {
        m_viewChanged = false;
    }
}

void Controller::updateScreen()
{
    // initialize display (ugly here, maybe static method in WatchyDisplay? maybe have class inherit from GxEPD2_BW<WatchyDisplay, WatchyDisplay::HEIGHT>?)

    GxEPD2_BW<WatchyDisplay, WatchyDisplay::HEIGHT>& screen = WatchyDisplay::getDisplay();
    screen.init(0, stateRepo.load().getInitialBoot(), 10, true);
    screen.setFullWindow();

    if (stateRepo.load().getInitialBoot()) {
        screen.fillScreen(GxEPD_BLACK);
        screen.setTextColor(GxEPD_WHITE);
    }

    bool partial = !stateRepo.load().getInitialBoot() && !m_viewChanged;

    switch (stateRepo.load().getViewState()) {
    case VIEW_STATE::WATCHFACE: {
        WatchFace watchFace;
        pcfTime time;
        rtc.getTimeDate(time);
        double vbat = gpio.getBatteryVoltage();

        watchFace.display(time, vbat, partial);
    } break;
    case VIEW_STATE::STEP: {
        StepView stepView;
        BMA456& bma456 = BMA456::getInstance();

        uint16_t steps = (uint16_t)bma456.getStepCounter();

        stepView.display(steps, partial);
    } break;

    default:
        break;
    }

    screen.hibernate();
}

void Controller::sleep()
{
    // configure next wake up
    uint64_t wakeupPinMask = 0;
    wakeupPinMask |= ((uint64_t)1 << CONST_PIN::BUTTON1);
    wakeupPinMask |= ((uint64_t)1 << CONST_PIN::BUTTON2);
    wakeupPinMask |= ((uint64_t)1 << CONST_PIN::BUTTON3);
    wakeupPinMask |= ((uint64_t)1 << CONST_PIN::BUTTON4);
    wakeupPinMask |= ((uint64_t)1 << CONST_PIN::BMA_INT1);
    wakeupPinMask |= ((uint64_t)1 << CONST_PIN::BMA_INT2);

    // wake up on RTC interrupt (active low)
    esp_sleep_enable_ext0_wakeup((gpio_num_t)CONST_PIN::RTC_INT, LOW);
    // wake up on any button press or BMA456 interrupt(active high)
    esp_sleep_enable_ext1_wakeup(wakeupPinMask, ESP_EXT1_WAKEUP_ANY_HIGH);

    // ... and sleep
    esp_deep_sleep_start();
}

Controller& Controller::getInstance()
{
    static Controller instance;
    return instance;
}
