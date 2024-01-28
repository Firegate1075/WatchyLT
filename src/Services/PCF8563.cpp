#include "PCF8563.h"

PCF8563::PCF8563()
{
}

/**
 * @brief This will reset all alarms and parameters of the RTC
 *
 */
void PCF8563::resetRTC()
{
    rtc.initClock();
}

/**
 * @brief Set Time and Date of RTC
 *
 * @param tm -> Struct containing time and date
 */
void PCF8563::setTimeDate(tmElements_t tm)
{
    // time_t t = makeTime(tm);
    // breakTime(t, tm);

    rtc.setDate(tm.Day, tm.Wday - 1, tm.Month, CONST_RTC::CENTURY, tmYearToY2k(tm.Year));
    Serial.print("Hour: ");
    Serial.println(tm.Hour);
    rtc.setTime(tm.Second, tm.Minute, tm.Hour);
    resetAlarm();
}

/**
 * @brief Retrieve time and data data from RTC
 *
 * @param tm -> Struct which will contain retrieved time and date
 */
void PCF8563::getTimeDate(tmElements_t& tm)
{
    rtc.getDate();

    tm.Year = rtc.getYear();
    tm.Month = rtc.getMonth();
    tm.Wday = rtc.getWeekday() + 1;

    tm.Day = rtc.getDay();
    tm.Hour = rtc.getHour();
    tm.Minute = rtc.getMinute();
    tm.Second = rtc.getSecond();
}

/**
 * @brief Clear alarm and set new for next minute
 *
 */
void PCF8563::resetAlarm()
{
    rtc.clearAlarm();
    uint8_t nextAlarmMin = rtc.getMinute() + 1;
    if (nextAlarmMin == 60) {
        nextAlarmMin = 0;
    }
    rtc.setAlarm(nextAlarmMin, 99, 99, 99);
}

char* PCF8563::formatTime()
{
    return rtc.formatTime();
}