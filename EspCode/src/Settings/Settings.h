#ifndef SETTINGS_H
#define SETTINGS_H

#include "RTCManager/RTCManager.h"
#include "DisplayManager/DisplayManager.h"
#include <ArduinoNvs.h>

class Settings {
  private:
    struct Config {
      char ssid[32];
      char password[32];
      bool useInternetTime;
      bool useSTA;
      int startDimTime;
      int endDimTime;
      int nightDim;
      int dayDim;
    } config;
    RTCManager *rtc;
    DisplayManager *display;

  public:
    // Constructor
    Settings(RTCManager *rtc, DisplayManager *display);

    // Load settings from NVS
    void loadSettings();
    void printSettings();

    // Save settings to NVS
    void saveSettings();
    void SetDateTimeToRTC(DateTime dateTime);

    // Setters
    void setSSID(const char* ssid);
    void setPassword(const char* password);
    void setUseInternetTime(bool useInternetTime);
    void setUseSTA(bool useSTA);
    bool setDimTimes(int startDimTime, int endDimTime);
    void setDimValues(int nightDim, int dayDim);

    // Getters
    const char* getSSID();
    const char* getPassword();
    bool getUseInternetTime();
    bool getUseSTA();
    void getDimTimes(int* startDimTime, int* endDimTime) { *startDimTime = config.startDimTime; *endDimTime = config.endDimTime; };
    void getDimValues(int* nightDim, int* dayDim) { *nightDim = config.nightDim; *dayDim = config.dayDim; };
};

#endif
