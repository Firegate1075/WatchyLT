#include "WatchFace.h"

WatchFace::WatchFace()
{
}

void WatchFace::display(const dateTime& td, double vbat, bool doPartial)
{
    screen.setFont(&Metropolis_Black24pt7b);

    char timeStr[6];
    makeTimeString(timeStr, td.Hour, td.Minute);

    // print time centered
    screen.setCursor(0, 60);
    printlnHorizontallyCentered(timeStr);

    // Change font for Date
    screen.setFont(&Metropolis_Black12pt7b);
    etl::string<25> date;

    // Print weekday name
    date = CONST_DAY_NAME::dayNamesEN[td.Weekday];
    printlnHorizontallyCentered(date.c_str(), -20); // Use offset, because font size was changed (remove blank space on display)

    // Print day of month and month name
    etl::to_string(td.Day, date, false);
    date += ". ";
    date += CONST_MONTH_NAME::monthNamesShortEN[td.Month];
    printlnHorizontallyCentered(date.c_str());

    // Print Year
    date = "";
    etl::to_string(td.Year, date, true);
    printlnHorizontallyCentered(date.c_str());

    // Print Battery voltage
    screen.setFont(&Metropolis_Black11pt7b);
    etl::string<5> vbatStr;
    etl::format_spec format;
    format.precision(2);

    etl::to_string(vbat, vbatStr, format);
    vbatStr += "V";
    printlnAlignedRight(vbatStr.c_str());
    screen.display(doPartial);
}

void WatchFace::makeTimeString(char* const str, uint8_t h, uint8_t m)
{
    str[0] = '0' + h / 10;
    str[1] = '0' + h % 10;
    str[2] = ':';
    str[3] = '0' + m / 10;
    str[4] = '0' + m % 10;
    str[5] = '\0';
}
