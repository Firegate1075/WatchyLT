#include "Controller.h"

RTC_DATA_ATTR bool initialBoot = true;
RTC_DATA_ATTR u_int8_t counter = 0;

Controller::Controller()
    : rtc(PCF8563::getInstance())
{
    Serial.begin(115200); // for debug
    Wire.begin(SDA, SCL);
    // handle initialBoot (wakeup after flashing)

    Serial.print("Initial boot: ");
    Serial.println(initialBoot);

    GxEPD2_BW<WatchyDisplay, (uint16_t)200U> display = WatchyDisplay::getDisplay();
    display.init(0, initialBoot, 10, true);
    display.setFullWindow();

    if (initialBoot) {
        // initialize sensors
        display.fillScreen(GxEPD_BLACK);
        display.setTextColor(GxEPD_WHITE);
        BMA456::getInstance().init();
        rtc.resetRTC();
    }

    rtc.resetAlarm();
    counter++;
    pcfTime td;
    rtc.getTimeDate(td);

    display.setFont(&Metropolis_Black11pt7b);
    display.setCursor(100, 100);
    display.println(String(counter).c_str());
    display.println(String(td.Hour).c_str());
    display.println(String(td.Minute).c_str());
    display.println(String(td.Second).c_str());
    display.display(!initialBoot);
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

    // wake up on RTC interrupt (active low)
    esp_sleep_enable_ext0_wakeup((gpio_num_t)CONST_PIN::RTC_INT, LOW);
    // wake up on any button press or BMA456 interrupt(active high)
    esp_sleep_enable_ext1_wakeup(wakeupPinMask, ESP_EXT1_WAKEUP_ANY_HIGH);

    initialBoot = false;
    display.hibernate();
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
        Serial.println("RTC Wakeup");
        // RTC wakeup
        break;
    case ESP_SLEEP_WAKEUP_EXT1:
        Serial.println("Button or BMA");
        // button interrupt or bma interrupt
        break;
    default:
        Serial.print("unexpected wakeup: ");
        Serial.println(cause);
        // error: unexpected wakeup
        break;
    }
}

Controller& Controller::getInstance()
{
    static Controller instance;
    return instance;
}
