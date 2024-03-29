#include "Controller.h"

RTC_DATA_ATTR bool initialBoot = true;

Controller::Controller()
    : rtc(PCF8563::getInstance())
    , gpio(GPIOHandler::getInstance())
    , viewObj(View::getInstance())
{
    Wire.begin(SDA, SCL);

    debugPrint("Initial boot: ");
    debugPrintln(initialBoot);

    // view.init(initialBoot);

    // handle initialBoot (wakeup after flashing)
    if (initialBoot) {
        // initialize sensors

        // BMA needs small delay to be setup correctly
        delay(1000);

        BMA456::getInstance().init();
        rtc.resetRTC();
    }

    rtc.resetAlarm();

    // TODO: set pin modes

    // configure wake up pins
    uint64_t wakeupPinMask = 0;
    wakeupPinMask |= ((uint64_t)1 << CONST_PIN::BUTTON1);
    wakeupPinMask |= ((uint64_t)1 << CONST_PIN::BUTTON2);
    wakeupPinMask |= ((uint64_t)1 << CONST_PIN::BUTTON3);
    wakeupPinMask |= ((uint64_t)1 << CONST_PIN::BUTTON4);
    wakeupPinMask |= ((uint64_t)1 << CONST_PIN::BMA_INT1);
    wakeupPinMask |= ((uint64_t)1 << CONST_PIN::BMA_INT2);

    // view.updateDisplay();
    handleWakeup();

    // wake up on RTC interrupt (active low)
    esp_sleep_enable_ext0_wakeup((gpio_num_t)CONST_PIN::RTC_INT, LOW);
    // wake up on any button press or BMA456 interrupt(active high)
    esp_sleep_enable_ext1_wakeup(wakeupPinMask, ESP_EXT1_WAKEUP_ANY_HIGH);

    initialBoot = false;
    // view.endScreen();
    delay(1000);

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

    switch (cause) {
    case ESP_SLEEP_WAKEUP_EXT0:
        debugPrintln("RTC Wakeup");
        // RTC wakeup
        break;
    case ESP_SLEEP_WAKEUP_EXT1:
        debugPrintln("Button or BMA");
        // button interrupt or bma interrupt
        break;
    default:
        debugPrint("unexpected wakeup: ");
        debugPrintln(cause);
        // error: unexpected wakeup
        break;
    }
}

Controller& Controller::getInstance()
{
    static Controller instance;
    return instance;
}
