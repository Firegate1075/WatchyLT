#include "Controller.h"
#include "Services/BMA456.h"
#include "constants.h"
#include <esp32-hal.h>
#include <esp_sleep.h>

RTC_DATA_ATTR bool initialBoot = true;

Controller::Controller()
{

    // handle initialBoot (wakeup after flashing)
    if (initialBoot) {
        // initialize sensors
        BMA456::getInstance().init();

        initialBoot = false;
    }

    // configure wake up pins
    uint64_t wakeupPinMask = 0;
    wakeupPinMask |= ((uint64_t) 1 << CONST_PIN::BUTTON1);
    wakeupPinMask |= ((uint64_t) 1 << CONST_PIN::BUTTON2);
    wakeupPinMask |= ((uint64_t) 1 << CONST_PIN::BUTTON3);
    wakeupPinMask |= ((uint64_t) 1 << CONST_PIN::BUTTON4);
    wakeupPinMask |= ((uint64_t) 1 << CONST_PIN::BMA_INT1);
    wakeupPinMask |= ((uint64_t) 1 << CONST_PIN::BMA_INT2);

    // wake up on RTC interrupt (active low)
    esp_sleep_enable_ext0_wakeup((gpio_num_t)CONST_PIN::RTC_INT, LOW);
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
    static Controller instance;
    return instance;
}
