#include "TimeManager.h"
#include <TimeLib.h>

// Constructor implementation
TimeManager::TimeManager(int offset)
    : timeClient(ntpUDP), CEST{"CEST", Last, Sun, Mar, 2, 120}, CET{"CET ", Last, Sun, Oct, 3, 60}, myTZ(CEST, CET), timeOffset(offset) {}

// Initialize the NTP client
void TimeManager::init(bool loadtimeFormNTP)
{
    if (loadtimeFormNTP)
    {
        timeClient.begin();
        timeClient.setTimeOffset(timeOffset); // Imposta il fuso orario
        Serial.println("TIME CLIENT INITIALIZED");
        //getting the time from the NTP server
        
    }
}

// Function to get the current date and time, and store it in the result array
int TimeManager::getDateTime(int resultArray[8])
{
    Serial.println("GETTING DATE AND TIME");
    while (!timeClient.update())
    {
        timeClient.forceUpdate();
    }
    Serial.println("TIME CLIENT UPDATED");

    // Get UTC time from NTP server
    time_t utcTime = timeClient.getEpochTime();

    // Convert UTC time to local time including daylight saving time (DST)
    TimeChangeRule *tcr;
    time_t localTime = myTZ.toLocal(utcTime, &tcr);

    // Extract components of local time
    tmElements_t tm;
    breakTime(localTime, tm);

    int hours = tm.Hour;
    int minutes = tm.Minute;
    int seconds = tm.Second;
    int day = tm.Wday;
    int year = tm.Year + 1970;
    int month = tm.Month;
    int dayInt = tm.Day;

    // Print local time
    Serial.print("LOCAL TIME: ");
    Serial.print(hours);
    Serial.print(":");
    Serial.print(minutes);
    Serial.print(":");
    Serial.println(seconds);
    Serial.print("DAY: ");
    Serial.println(day);

    // Print local date
    Serial.print("DATE: ");
    Serial.print(dayInt);
    Serial.print("/");
    Serial.print(month);
    Serial.print("/");
    Serial.println(year);

    // Determine the day of the week using the weekday() function
    int dayOfWeek = weekday(localTime);

    // Print day of the week
    Serial.print("DAY OF THE WEEK: ");
    Serial.println(dayOfWeek);

    // Store results in the array
    resultArray[0] = hours;
    resultArray[1] = minutes;
    resultArray[2] = seconds;
    resultArray[3] = day;
    resultArray[4] = year;
    resultArray[5] = month;
    resultArray[6] = dayInt;
    resultArray[7] = dayOfWeek;

    return localTime;
}
