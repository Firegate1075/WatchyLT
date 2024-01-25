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