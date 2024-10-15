#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <Arduino.h>
#include <WiFi.h>
#include "Settings/Settings.h"

class WiFiManager {
public:
    WiFiManager(Settings *settings);
    bool initWIFI(bool useSTA);
    //function that returns the IP address of the ESP32 as a string
    String getLocalIP();

private:
    Settings *settings;
};

#endif // WIFIMANAGER_H