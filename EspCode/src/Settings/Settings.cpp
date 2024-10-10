#include "Settings.h"

// Constructor
Settings::Settings(RTCManager *rtc) : rtc(rtc) {
  EEPROM.begin(EEPROM_SIZE);
  loadSettings();
}

// Load settings from EEPROM
void Settings::loadSettings() {
  EEPROM.get(EEPROM_ADDRESS, config);
  // load a first time the settings
  config.useSTA = false;
  config.useInternetTime = false;
  config.ssid[0] = '\0';
  config.password[0] = '\0';
  saveSettings();
}

// Save settings to EEPROM
void Settings::saveSettings() {
  EEPROM.put(EEPROM_ADDRESS, config);
  EEPROM.commit();
  // reboot the device to apply the new settings
  delay(1000);
  //ESP.restart();
}

void Settings::SetDateTimeToRTC(DateTime dateTime)
{
  rtc->setManualTime(dateTime.year(), dateTime.month(), dateTime.day(), dateTime.hour(), dateTime.minute(), dateTime.second(), dateTime.dayOfTheWeek());
}

// Setters
void Settings::setSSID(const char* ssid) {
  strncpy(config.ssid, ssid, sizeof(config.ssid));
}

void Settings::setPassword(const char* password) {
  strncpy(config.password, password, sizeof(config.password));
}

void Settings::setUseInternetTime(bool useInternetTime) {
  config.useInternetTime = useInternetTime;
}

void Settings::setUseSTA(bool useSTA)
{
    config.useSTA = useSTA;
}

// Getters
const char* Settings::getSSID() {
  return config.ssid;
}

const char* Settings::getPassword() {
  return config.password;
}

bool Settings::getUseInternetTime() {
  return config.useInternetTime;
}

bool Settings::getUseSTA()
{
    return config.useSTA;
}
