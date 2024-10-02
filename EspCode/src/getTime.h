#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Timezone.h> // Includi la libreria Timezone.h

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

// Definisci il fuso orario e i parametri relativi all'ora legale e all'ora solare per Roma
TimeChangeRule CEST = {"CEST", Last, Sun, Mar, 2, 120};  // Central European Summer Time
TimeChangeRule CET = {"CET ", Last, Sun, Oct, 3, 60};     // Central European Time
Timezone myTZ(CEST, CET);

// Variables to save date and time
String formattedDate;
String dayStamp;
String timeStamp;

void initgetTime() {
  // Initialize a NTPClient to get time
  timeClient.begin();
  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600
  // GMT +8 = 28800
  // GMT -1 = -3600
  // GMT 0 = 0
  timeClient.setTimeOffset(0);
  Serial.println("TIME CLIENT INITIALIZED");
}

// Function to get date and time from NTP server
int getDateTime(int resultArray[6]) {
  Serial.println("GETTING DATE AND TIME");
  while (!timeClient.update()) {

    timeClient.forceUpdate();
  }
  Serial.println("TIME CLIENT UPDATED");
  // Get UTC time from NTP server
  time_t utcTime = timeClient.getEpochTime();
  
  // Convert UTC time to local time including daylight saving time (DST) and standard time
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
