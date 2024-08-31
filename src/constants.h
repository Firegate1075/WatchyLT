#pragma once

#include <Arduino.h>
#include <GxEPD2.h>

namespace CONST_CREDENTIALS {
constexpr size_t SSID_LEN = 32;
constexpr size_t PASS_LEN = 64;
constexpr size_t MAX_CREDENTIALS = 8;
constexpr const char SSID_KEY[] = "ssid";
constexpr const char PASS_KEY[] = "pass";
} // namespace CONST_CREDENTIALS

namespace CONST_MENU {
constexpr size_t MAX_MENU_SIZE = 8;
}

namespace CONST_PIN {
constexpr uint8_t BUTTON1 = 26;
constexpr uint8_t BUTTON2 = 25;
constexpr uint8_t BUTTON3 = 35;
constexpr uint8_t BUTTON4 = 4;

constexpr uint8_t RTC_INT = 27;

constexpr uint8_t BMA_INT1 = 14;
constexpr uint8_t BMA_INT2 = 12;

constexpr int16_t CS = 5;
constexpr int16_t DC = 10;
constexpr int16_t RES = 9;
constexpr int16_t BUSY = 19;

constexpr u_int8_t BATT_ADC = 34;
}

namespace CONST_BUTTON {
constexpr uint8_t BACK = 1 << 0;
constexpr uint8_t MENU = 1 << 1;
constexpr uint8_t UP = 1 << 2;
constexpr uint8_t DOWN = 1 << 3;
constexpr uint32_t HOLD_TIME_MS = 1000;
}

namespace CONST_WIFI {
constexpr uint8_t MAX_OBSERVERS = 3;
constexpr uint8_t portalTimeout = 60;
constexpr uint8_t connectTimeout = 30;
constexpr const char* configurationPortalName = "Watchy-AP";
}

namespace CONST_NETWORK {
constexpr uint8_t IP_LENGTH = 16;
constexpr uint8_t URL_LENGTH = 100;
constexpr const char timeApiURL[] = "https://timeapi.io/api/Time/current/ip";
constexpr const char ipApiURL[] = "https://api.ipify.org/?format=text";
constexpr int32_t IP_TIMEOUT = 10000;
constexpr const char AP_IP_ADDRESS[] = "192.168.4.1";
}

namespace CONST_DISPLAY {
constexpr uint16_t WIDTH = 200;
constexpr uint16_t WIDTH_VISIBLE = WIDTH;
constexpr uint16_t HEIGHT = 200;
constexpr GxEPD2::Panel panel = GxEPD2::Panel::GDEH0154D67;
constexpr bool hasColor = false;
constexpr bool hasPartialUpdate = true;
constexpr bool hasFastPartialUpdate = true;
constexpr uint16_t power_on_time = 100; // ms, e.g. 95583us
constexpr uint16_t power_off_time = 150; // ms, e.g. 140621us
constexpr uint16_t full_refresh_time = 2600; // ms, e.g. 2509602us
constexpr uint16_t partial_refresh_time = 500; // ms, e.g. 457282us

constexpr int16_t BUSY_LEVEL = HIGH;
constexpr uint32_t BUSY_TIMEOUT = 10000000;
}

namespace CONST_SPI {
constexpr uint32_t CLOCK = 20000000;
constexpr uint8_t BIT_ORDER = MSBFIRST;
constexpr uint8_t DATA_MODE = SPI_MODE0;
}

namespace CONST_RTC {
constexpr uint8_t CENTURY = 0; // 0 = 2000; 1 = 1900;
constexpr uint8_t REDUCED_WAKEUP_MINUTES = 5; // wakes up every 5 minutes when not moved
}

namespace CONST_BMA {
constexpr uint8_t BMA456_DEVICE_ADDR = 0x18;
constexpr uint16_t ANYMOT_DURATION = 5; // x20ms = 100ms, max 163s
constexpr uint16_t ANYMOT_THRESHOLD = 0xaa; // default
constexpr uint16_t NOMOT_DURATION = 2 * 60 * 20; // x20ms = 10s, max 163s (13bit?)
constexpr uint16_t NOMOT_THRESHOLD = 10; // .48mG, might be too small
}

namespace CONST_PREFERENCES {
constexpr const char CREDENTIALS_NAMESPACE[] = "cred";
}

enum class VIEW_STATE_UID {
    WATCHFACE,
    STEP,
    CONFIG_PORTAL
};

namespace CONST_MONTH_NAME {
constexpr const char* monthNamesEN[13] = {
    "ERROR", "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"
};
constexpr const char* monthNamesShortEN[13] = {
    "ERROR", "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};
};

namespace CONST_DAY_NAME {
constexpr const char* dayNamesEN[7] = {
    "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"
};
constexpr const char* dayNamesShortEN[7] = {
    "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"
};
};

typedef struct {
    uint8_t Second; // 0-59
    uint8_t Minute; // 0-59
    uint8_t Hour; // 0-23
    uint8_t Weekday; // 0-6
    uint8_t Day; // 1-31
    uint8_t Month; // 1-12
    uint8_t Year; // 2000 - 2099
} dateTime;
