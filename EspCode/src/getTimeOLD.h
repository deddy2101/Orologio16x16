


#include <NTPClient.h>
#include <WiFiUdp.h>


WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

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
  timeClient.setTimeOffset(3600);
  Serial.println("TIME CLIENT INITIALIZED");
}
int getDateTime(int resultArray[6]) {
  while(!timeClient.update()) {
    timeClient.forceUpdate();
  }
  int hours = timeClient.getHours();
    int minutes = timeClient.getMinutes();
    int seconds = timeClient.getSeconds();
    int day = timeClient.getDay();
    //print time to serial monitor
    Serial.print("TIME: ");
    Serial.print(hours);
    Serial.print(":");
    Serial.print(minutes);
    Serial.print(":");
    Serial.println(seconds);
    Serial.print("DAY: ");
    Serial.println(day);
    String DateAndTime = timeClient.getFormattedDate();

    //we need to split the string into date and time
    int splitT = DateAndTime.indexOf("T");
    String dateStamp = DateAndTime.substring(0, splitT);
    //now we have the date we split the date into individual parts
    int splitD = dateStamp.indexOf("-");
    String yearStamp = dateStamp.substring(0, splitD);
    String monthStamp = dateStamp.substring(splitD+1, dateStamp.length());
    int splitM = monthStamp.indexOf("-");
    String dayStamp = monthStamp.substring(splitM+1, monthStamp.length());
    monthStamp = monthStamp.substring(0, splitM);
    //now we have the date split into its parts we can print them out
    Serial.print("DATE: ");
    Serial.print(dayStamp);
    Serial.print("/");
    Serial.print(monthStamp);
    Serial.print("/");
    Serial.println(yearStamp);
     // Converte le stringhe in interi
  int year = yearStamp.toInt();
  int month = monthStamp.toInt();
  int dayInt = dayStamp.toInt();
      int dayOfWeek = ((dayInt + 2*month + 3*(month + 1)/5 + year + year/4 - year/100 + year/400) % 7) + 1;

    resultArray[0] = hours;
    resultArray[1] = minutes;
    resultArray[2] = seconds;
    resultArray[3] = day;
    resultArray[4] = year;
    resultArray[5] = month;
    resultArray[6] = dayInt;
    resultArray[7] = dayOfWeek;

    //calculate epoch time
    int epoch = timeClient.getEpochTime();
    return epoch;


}