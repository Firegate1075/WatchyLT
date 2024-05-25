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
void PCF8563::setTimeDate(pcfTime tm)
{
    // time_t t = makeTime(tm);
    // breakTime(t, tm);

    rtc.setDate(tm.Day, tm.Weekday, tm.Month, CONST_RTC::CENTURY, tm.Year);
    rtc.setTime(tm.Hour, tm.Minute, tm.Second);
    resetAlarm();
}

/**
 * @brief Retrieve time and data data from RTC
 *
 * @param tm -> Struct which will contain retrieved time and date
 */
void PCF8563::getTimeDate(pcfTime& tm)
{
    rtc.getDate();
    rtc.getTime();

    tm.Year = rtc.getYear();
    tm.Month = rtc.getMonth();
    tm.Weekday = rtc.getWeekday();

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
    rtc.getTime();
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