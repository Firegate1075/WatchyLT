#include "Controller.h"
#include "Services/BMA456.h"
#include <esp32-hal.h>
#include <esp_sleep.h>

RTC_DATA_ATTR bool initialBoot = true;

Controller* Controller::instancePointer = NULL;

Controller::Controller(pin_config_t pinConfiguration)
{
    pinConfig = pinConfiguration;
    // handle initialBoot (wakeup after flashing)
    if (initialBoot) {
        // initialize sensors
        BMA456::getInstance().init(pinConfig.bmaInt1, pinConfig.bmaInt2);
    }

    // configure wake up pins
    uint64_t wakeupPinMask;
    wakeupPinMask |= (1 << pinConfig.button1);
    wakeupPinMask |= (1 << pinConfig.button2);
    wakeupPinMask |= (1 << pinConfig.button3);
    wakeupPinMask |= (1 << pinConfig.button4);
    wakeupPinMask |= (1 << pinConfig.bmaInt1);

    // wake up on RTC interrupt (active low)
    esp_sleep_enable_ext0_wakeup((gpio_num_t)pinConfig.rtcInt, LOW);
    // wake up on any button press or BMA456 interrupt(active high)
    esp_sleep_enable_ext1_wakeup(wakeupPinMask, ESP_EXT1_WAKEUP_ANY_HIGH);

    // configure sensors (or maybe only before use ?)
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
        // RTC wakeup
        break;
    case ESP_SLEEP_WAKEUP_EXT1:
        // button interrupt or bma interrupt
        break;
    default:
        // error: unexpected wakeup
        break;
    }
}

Controller& Controller::getInstance()
{
    // TODO: insert return statement here
    return *instancePointer;
}

Controller& Controller::createInstance(const pin_config_t pinConfiguration)
{
    if (instancePointer == NULL) {
        instancePointer = new Controller(pinConfiguration);
    }
    return *instancePointer;
}