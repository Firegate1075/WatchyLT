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
void PCF8563::setTimeDate(dateTime tm)
{
    // PFC only stores "00" to "99" as year
    rtc.setDate(tm.Day, tm.Weekday, tm.Month, CONST_RTC::CENTURY, tm.Year - 2000);
    rtc.setTime(tm.Hour, tm.Minute, tm.Second);
    resetAlarm();
}

/**
 * @brief Retrieve time and data data from RTC
 *
 * @param tm -> Struct which will contain retrieved time and date
 */
void PCF8563::getTimeDate(dateTime& tm)
{
    rtc.getDate();
    rtc.getTime();

    tm.Year = rtc.getYear() + 2000; // PFC only stores "00" to "99"
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

/**
 * @brief Clear alarm. PCF8563 interrupt will no longer fire until next alarm.
 */
void PCF8563::clearAlarm()
{
    rtc.clearAlarm();
}

/**
 * @brief sets the Alarm. An Alarm will be triggered in minutesToWakeup minutes. Default=1
 */
void PCF8563::setAlarm(uint8_t minutesToWakeup)
{
    rtc.getTime();

    uint8_t nextAlarmMin = rtc.getMinute() + minutesToWakeup;
    nextAlarmMin %= 60; // wrap around hours

    rtc.setAlarm(nextAlarmMin, 99, 99, 99);
}

char* PCF8563::formatTime()
{
    return rtc.formatTime();
}