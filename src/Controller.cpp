#include "Controller.h"

Controller::Controller()
    : rtc(PCF8563::getInstance())
    , gpio(GPIOHandler::getInstance())
    , stateRepo(StateRepository::getInstance())
{
    Wire.begin(SDA, SCL);

    debugPrintln("---------------------\nBoot");
    debugPrintf("initialBoot=%d\n", stateRepo.load().getInitialBoot());
    debugPrintf("inMotion=%d\n", stateRepo.load().isInMotion());
    debugPrintf("viewState=%d\n", (uint8_t)stateRepo.load().getViewStateID());

    // handle initialBoot (wakeup after flashing)
    if (stateRepo.load().getInitialBoot()) {
        // initialize sensors

        // BMA needs small delay to be setup correctly

        BMA456& bma456 = BMA456::getInstance();

        bma456.init();
        bma456.enableStepCounter();
        bma456.setStepCounterWatermark(1);
        // bma456.setStepCountInterruptEnable(); enable only when necessary (step screen)

        bma456.setWristInterruptEnable();
        bma456.setNoMotionInterruptEnable();

        rtc.resetRTC();
        rtc.resetAlarm();
        StateModel stateModel = stateRepo.load();
        stateModel.setViewState(VIEW_STATE_UID::CONFIG_PORTAL);
        stateModel.setInMotion(true);
        stateRepo.save(stateModel);

        m_currentViewState = ViewStateFactory::createFromUID(VIEW_STATE_UID::CONFIG_PORTAL);
        m_currentViewState->onEnter();
    } else {
        m_currentViewState = ViewStateFactory::createFromUID(stateRepo.load().getViewStateID());
    }
}

void Controller::setViewState(VIEW_STATE_UID newStateID)
{
    m_currentViewState->onExit();
    m_currentViewState = ViewStateFactory::createFromUID(stateRepo.load().getViewStateID());
    m_currentViewState->onEnter();
}

/**
 * @brief handler method for proccessing different wakeup reasons
 *
 */
void Controller::handleWakeup()
{
    // retrieve cause of wakeup
    esp_sleep_wakeup_cause_t cause = esp_sleep_get_wakeup_cause();
    BMA456& bma456 = BMA456::getInstance();
    BMA456_interrupt bma_interrupt = bma456.getInterrupt();
    StateModel model = stateRepo.load();

    switch (cause) {
    case ESP_SLEEP_WAKEUP_EXT0:
        debugPrintln("RTC Wakeup");
        rtc.clearAlarm();
        if (stateRepo.load().isInMotion())
            rtc.setAlarm(1);
        else
            rtc.setAlarm(CONST_RTC::REDUCED_WAKEUP_MINUTES);

        // RTC wakeup
        break;
    case ESP_SLEEP_WAKEUP_EXT1:
        if (bma_interrupt != BMA456_interrupt::NONE) {
            debugPrintln("BMA wakeup");
            switch (bma_interrupt) {
            case BMA456_interrupt::ANY_MOT:
                debugPrintln("any mot int");
                stateRepo.save(model.setInMotion(true));

                bma456.setNoMotionInterruptEnable(true);
                bma456.setAnyMotionInterruptEnable(false);
                bma456.disableAnyMotionDetection();
                bma456.enableStepCounter();

                // activate minutely alarms
                rtc.clearAlarm();
                rtc.setAlarm(1);

                break;
            case BMA456_interrupt::NO_MOT:
                debugPrintln("no mot int");
                stateRepo.save(model.setInMotion(false));
                bma456.setAnyMotionInterruptEnable(true);
                bma456.setNoMotionInterruptEnable(false);
                bma456.disableNoMotionDetection();
                bma456.disableStepCounter();

                // activate reduced alarms
                rtc.clearAlarm();
                rtc.setAlarm(CONST_RTC::REDUCED_WAKEUP_MINUTES);
                break;
            default:
                debugPrint("interrupt=");
                debugPrintln((uint8_t)bma_interrupt);
                break;
            }
        } else {
            debugPrintln("Button wakeup");
            stateRepo.save(model.setInMotion(true));
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
    VIEW_STATE_UID currentState = stateRepo.load().getViewStateID();

    uint8_t buttons = gpio.readButtons();

    VIEW_STATE_UID nextState = m_currentViewState->handleButtons(buttons);

    if (nextState != currentState) {
        m_viewChanged = true;

        // save new view state
        StateModel stateModel = stateRepo.load();
        stateModel.setViewState(nextState);
        stateRepo.save(stateModel);

        setViewState(nextState);
    }
}

void Controller::handleRadio()
{
    WifiHandler& wifi = WifiHandler::getInstance();
    if (wifi.isConfigurationPortalOpen()) {
        wifi.loop();
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

    m_currentViewState->updateScreen(partial);

    m_viewChanged = false;

    screen.hibernate();
}

void Controller::sleep()
{
    // set initialBoot false
    StateModel model = stateRepo.load();
    stateRepo.save(model.setInitialBoot(false));

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

    debugPrintln("Entering deep sleep!\n");

    // ... and sleep
    esp_deep_sleep_start();
}

bool Controller::isBusy()
{
    return m_currentViewState->isBusy();
}

Controller& Controller::getInstance()
{
    static Controller instance;
    return instance;
}
