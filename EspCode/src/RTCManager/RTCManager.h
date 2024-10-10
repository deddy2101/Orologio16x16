#ifndef RTCMANAGER_H
#define RTCMANAGER_H

#include <Wire.h>
#include "DS3231.h" // Libreria DS3231 per il modulo RTC

class RTCManager {
  private:
    DS3231 rtc;
    RTClib myRTC;

  public:
    // Constructor
    RTCManager();

    // Set the time using NTP or manually
    void setTimeFromNTP(int datetime[8]);    // Set time from NTP server
    void setManualTime(int year, int month, int day, int hour, int minute, int second, int dayOfWeek);

    // Get the current time and date
    DateTime getCurrentTime();

    // Print current time to the serial monitor
    void printCurrentTime();

    // Set RTC day of the week
    void setDayOfWeek(int dayOfWeek);
};

#endif
