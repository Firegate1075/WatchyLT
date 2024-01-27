#pragma once

#include <Arduino.h>

namespace CONST_CREDENTIALS {
constexpr size_t SSID_LEN = 32;
constexpr size_t PASS_LEN = 64;
constexpr size_t MAX_CREDENTIALS = 8;
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
}

namespace CONST_WIFI {
constexpr uint8_t MAX_OBSERVERS = 3;
constexpr uint8_t portalTimeout = 60;
constexpr uint8_t connectTimeout = 30;
constexpr char* configurationPortalName = "Watchy-AP";
}