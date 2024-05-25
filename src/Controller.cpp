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
        stateModel.setViewState(VIEW_STATE::CONFIG_PORTAL);
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
    case VIEW_STATE::CONFIG_PORTAL: {
        if (buttons & BACK) {
            nextState = VIEW_STATE::WATCHFACE;

            debugPrintln("closing WIFI");
            WifiHandler& wifi = WifiHandler::getInstance();
            CredentialRepository& credentialRepo = CredentialRepository::getInstance();

            CredentialModel newModel = wifi.getCredentialsOfCurrentNetwork();
            const etl::vector<CredentialModel, CONST_CREDENTIALS::MAX_CREDENTIALS>& vec = credentialRepo.loadAll();
            bool alreadySaved = false;
            for (CredentialModel model : vec) {
                if (model.getSSID() == newModel.getSSID()) {
                    alreadySaved = true;
                    debugPrint("Wifi with SSID=");
                    debugPrint(newModel.getSSID().c_str());
                    debugPrintln(" was already saved");
                    break;
                }
            }

            if (!alreadySaved) {
                credentialRepo.save(newModel);
                debugPrint("Wifi with SSID=");
                debugPrint(newModel.getSSID().c_str());
                debugPrintln(" saved");
            }

            wifi.closeConfigurationPortal();

            debugPrintln("connecting");
            if (wifi.connectToNetwork(credentialRepo.loadAll())) {
                debugPrintln("connected");
                // sync time while connected
                NTPHandler ntp;
                pcfTime currentTime = ntp.getTime();
                rtc.setTimeDate(currentTime);
            }

            m_busy = false; // we may sleep now
        }
    } break;

    default:
        if (buttons) {
            nextState = VIEW_STATE::WATCHFACE;
        }
        break;
    }

    if (nextState != currentState) {
        m_viewChanged = true;

        // save new view state
        StateModel stateModel = stateRepo.load();
        stateModel.setViewState(nextState);
        stateRepo.save(stateModel);
    }
}

void Controller::handleRadio()
{
    VIEW_STATE currentState = stateRepo.load().getViewState();

    switch (currentState) {
    case VIEW_STATE::WATCHFACE: {

    } break;
    case VIEW_STATE::STEP: {

    } break;
    case VIEW_STATE::CONFIG_PORTAL: {
        WifiHandler& wifi = WifiHandler::getInstance();

        wifi.openConfigurationPortal();
        wifi.loop();
    } break;

    default:

        break;
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
    case VIEW_STATE::CONFIG_PORTAL: {
        ConfigPortalView view;
        view.display(partial);
        m_busy = true; // prevent Watchy from sleeping while config portal is open
    } break;

    default:
        break;
    }

    m_viewChanged = false;

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

bool Controller::isBusy()
{
    return m_busy;
}

Controller& Controller::getInstance()
{
    static Controller instance;
    return instance;
}
