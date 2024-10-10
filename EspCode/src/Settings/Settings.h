#ifndef SETTINGS_H
#define SETTINGS_H

#include <EEPROM.h>
#include "RTCManager/RTCManager.h"

class Settings {
  private:
    struct Config {
      char ssid[32];
      char password[32];
      bool useInternetTime;
      bool useSTA;
    } config;
    RTCManager *rtc;
    const int EEPROM_SIZE = sizeof(Config);
    const int EEPROM_ADDRESS = 0;

  public:
    // Constructor
    Settings(RTCManager *rtc);

    // Load settings from EEPROM
    void loadSettings();

    // Save settings to EEPROM
    void saveSettings();
    void SetDateTimeToRTC(DateTime dateTime);

    // Setters
    void setSSID(const char* ssid);
    void setPassword(const char* password);
    void setUseInternetTime(bool useInternetTime);
    void setUseSTA(bool useSTA);

    // Getters
    const char* getSSID();
    const char* getPassword();
    bool getUseInternetTime();
    bool getUseSTA();
};

#endif
