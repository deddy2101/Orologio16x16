#include "RTCManager.h"
#include <Wire.h>

// Constructor
RTCManager::RTCManager() {
}

void RTCManager::init()
{
        Wire.begin(33, 35);  // Inizializzazione I2C con i pin specifici per ESP32

    //check if there is a RTC connected making a request to the RTC like the temperature
    if (rtc.getTemperature() == 0)
    {
        Serial.println("RTC not found!");
        while (1)
            ;
    }
    else
    {
        Serial.println("RTC found!");
    }

}

// Set the time from NTP server
void RTCManager::setTimeFromNTP(int datetime[8]) {
    rtc.setEpoch(datetime[0]); // Imposta l'ora dall'epoch NTP
    setDayOfWeek(datetime[7]); // Imposta il giorno della settimana
}

// Set the time manually
void RTCManager::setManualTime(int year, int month, int day, int hour, int minute, int second, int dayOfWeek) {
    DateTime now = DateTime(year, month, day, hour, minute, second);
    rtc.setEpoch(now.unixtime());  // Imposta l'orologio
    setDayOfWeek(dayOfWeek);       // Imposta il giorno della settimana
}

// Get the current time and date
DateTime RTCManager::getCurrentTime() {
    return myRTC.now();  // Restituisce l'oggetto DateTime corrente
}

// Print the current time to the serial monitor
void RTCManager::printCurrentTime() {
    DateTime now = getCurrentTime();
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.print(" ");
    Serial.print(now.day(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.year(), DEC);
    Serial.print(" ");
    Serial.print(now.dayOfTheWeek(), DEC);
    Serial.println();
}

// Set the RTC day of the week
void RTCManager::setDayOfWeek(int dayOfWeek) {
    rtc.setDoW(dayOfWeek);
}
