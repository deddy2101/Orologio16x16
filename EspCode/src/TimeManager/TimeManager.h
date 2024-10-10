#ifndef TIMEMANAGER_H
#define TIMEMANAGER_H

#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Timezone.h> // Includi la libreria Timezone.h

class TimeManager {
  private:
    WiFiUDP ntpUDP;
    NTPClient timeClient;
    TimeChangeRule CEST;  // Central European Summer Time
    TimeChangeRule CET;   // Central European Time
    Timezone myTZ;
    int timeOffset;

  public:
    // Constructor
    TimeManager(int offset);

    // Initialize the NTP client
    void init(bool loadtimeFormNTP);

    // Function to get the current date and time, and store it in the result array
    int getDateTime(int resultArray[8]);
};

#endif
