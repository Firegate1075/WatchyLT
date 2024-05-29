#include "NTPHandler.h"

void NTPHandler::wifiCallback()
{
}

etl::string<CONST_NETWORK::IP_LENGTH> NTPHandler::getIP()
{
    Serial.print("Getting IP. URL=");
    http.begin(CONST_NETWORK::ipApiURL);
    Serial.println(CONST_NETWORK::ipApiURL);
    http.GET();
    String ip = http.getString();
    Serial.print("ip=");
    Serial.println(ip);
    return etl::string<CONST_NETWORK::IP_LENGTH>(ip.c_str());
}

pcfTime NTPHandler::getTime()
{
    auto ip = getIP();
    Serial.print("Getting time. URL=");

    etl::string<CONST_NETWORK::URL_LENGTH> url = CONST_NETWORK::timeApiURL;
    url += "?ipAddress=";
    url += ip;

    http.begin(url.c_str());

    Serial.println(url.c_str());
    http.GET();
    String responseString = http.getString();
    Serial.println(responseString);
    JsonDocument response;
    deserializeJson(response, responseString.c_str());

    pcfTime time;

    uint16_t yearRaw = response["year"];
    time.Year = ((uint8_t)yearRaw - 2000); // time.Year saves only 00..99 of year
    time.Month = response["month"];
    time.Day = response["day"];
    time.Hour = response["hour"];
    time.Minute = response["minute"];
    time.Second = response["seconds"];
    String weekdayString = response["dayOfWeek"]; // maybe implement with etl string

    if (weekdayString == "Sunday")
        time.Weekday = 0;
    else if (weekdayString == "Monday")
        time.Weekday = 1;
    else if (weekdayString == "Tuesday")
        time.Weekday = 2;
    else if (weekdayString == "Wednesday")
        time.Weekday = 3;
    else if (weekdayString == "Thursday")
        time.Weekday = 4;
    else if (weekdayString == "Friday")
        time.Weekday = 5;
    else if (weekdayString == "Saturday")
        time.Weekday = 6;
    else
        time.Weekday = -1;

    return time;
}
