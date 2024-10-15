#include "Settings.h"

// Constructor
Settings::Settings(RTCManager *rtc, DisplayManager *display) : rtc(rtc), display(display) {
  NVS.begin();  // Inizializza NVS
  loadSettings();
}

// Load settings from NVS
void Settings::loadSettings() {
  // Carica SSID
  String ssid = NVS.getString("ssid");
  if (ssid.length() > 0) {
    memset(config.ssid, 0, sizeof(config.ssid));
    memcpy(config.ssid, ssid.c_str(), sizeof(config.ssid) - 1);
  }

  // Carica Password
  String password = NVS.getString("password");
  if (password.length() > 0) {
    memset(config.password, 0, sizeof(config.password));
    memcpy(config.password, password.c_str(), sizeof(config.password) - 1);
  }

  // Carica flag booleani
  config.useInternetTime = NVS.getInt("useInternetTime");
  config.useSTA = NVS.getInt("useSTA");

  // Carica orari di dim
  config.startDimTime = NVS.getInt("startDimTime", 22);  // default 22
  config.endDimTime = NVS.getInt("endDimTime", 7);  // default 7

  // Carica valori di dim
  config.nightDim = NVS.getInt("nightDim", 0);  // default 0
  config.dayDim = NVS.getInt("dayDim", 255);  // default 255

  printSettings();
}

// Save settings to NVS
void Settings::saveSettings() {
  // Salva SSID
  NVS.setString("ssid", config.ssid);

  // Salva Password
  NVS.setString("password", config.password);

  // Salva flag booleani
  NVS.setInt("useInternetTime", config.useInternetTime ); // salva config.useInternetTime
  NVS.setInt("useSTA", config.useSTA); // salva config.useSTA

  // Salva orari di dim
  NVS.setInt("startDimTime", config.startDimTime);
  NVS.setInt("endDimTime", config.endDimTime);

  // Salva valori di dim
  NVS.setInt("nightDim", config.nightDim);
  NVS.setInt("dayDim", config.dayDim);

  NVS.commit();  // Assicurati di salvare le modifiche
    display->blinkSavedSettings();

  printSettings();
}

void Settings::SetDateTimeToRTC(DateTime dateTime)
{
  rtc->setManualTime(dateTime.year(), dateTime.month(), dateTime.day(), dateTime.hour(), dateTime.minute(), dateTime.second(), dateTime.dayOfTheWeek());
  display->blinkSavedSettings();
}

void Settings::setSSID(const char* ssid) {
  memset(config.ssid, 0, sizeof(config.ssid));
  memcpy(config.ssid, ssid, min(strlen(ssid), sizeof(config.ssid) - 1));
  Serial.println("Set SSID");
  Serial.println(config.ssid);
}

void Settings::setPassword(const char* password) {
  memset(config.password, 0, sizeof(config.password));
  memcpy(config.password, password, min(strlen(password), sizeof(config.password) - 1));
  Serial.println("Set Password");
  Serial.println(config.password);
}

void Settings::setUseInternetTime(bool useInternetTime) {
  config.useInternetTime = useInternetTime;
  Serial.println("Set Use Internet Time");
  Serial.println(config.useInternetTime);
}

void Settings::setUseSTA(bool useSTA) {
  config.useSTA = useSTA;
  Serial.println("Set Use STA");
  Serial.println(config.useSTA);
}

bool Settings::setDimTimes(int startDimTime, int endDimTime) {
  if (startDimTime <= 24 && endDimTime <= 24 && startDimTime >= 0 && endDimTime >= 0) {
    config.startDimTime = startDimTime;
    config.endDimTime = endDimTime;
    return true;
  } else {
    Serial.println("Time must be between 0 and 24");
    return false;
  }
}

void Settings::setDimValues(int nightDim, int dayDim) {
  config.nightDim = nightDim;
  config.dayDim = dayDim;
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

bool Settings::getUseSTA() {
  return config.useSTA;
}

void Settings::printSettings() {
  Serial.println("Settings:");
  Serial.print("SSID: ");
  Serial.println(config.ssid);
  Serial.print("Password: ");
  Serial.println(config.password);
  Serial.print("Use internet time: ");
  Serial.println(config.useInternetTime);
  Serial.print("Use STA: ");
  Serial.println(config.useSTA);
  Serial.print("Start dim time: ");
  Serial.println(config.startDimTime);
  Serial.print("End dim time: ");
  Serial.println(config.endDimTime);
  Serial.println();
  Serial.println("Dim values: ");
  Serial.println(config.nightDim);
  Serial.println(config.dayDim);
  Serial.println();
}
