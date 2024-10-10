#include "Settings.h"

// Constructor
Settings::Settings(RTCManager *rtc) : rtc(rtc) {
  EEPROM.begin(EEPROM_SIZE);
  loadSettings();
  config.startDimTime = 22;
  config.endDimTime = 7;
  //save a sample string in the settings
}

// Load settings from EEPROM
void Settings::loadSettings() {
  EEPROM.get(EEPROM_ADDRESS, config);
  // load a first time the settings
  //printSettings();
  Serial.println("Settings loaded");
  Serial.print("SSID: ");
  Serial.println(config.ssid);
  Serial.print("Password: ");
  Serial.println(config.password);
  Serial.print("Use internet time: ");
  Serial.println(config.useInternetTime);
  Serial.print("Use STA: ");
  Serial.println(config.useSTA);

}

// Save settings to EEPROM
void Settings::saveSettings() {
  EEPROM.put(EEPROM_ADDRESS, config);
  EEPROM.commit();
  
  printSettings();

  Config test;
  EEPROM.get(EEPROM_ADDRESS, test);
  Serial.println("Settings saved retrived");
  Serial.print("SSID: ");
  Serial.println(test.ssid);
  Serial.print("Password: ");
  Serial.println(test.password);
  Serial.print("Use internet time: ");
  Serial.println(test.useInternetTime);
  Serial.print("Use STA: ");
  Serial.println(test.useSTA);


  //ESP.restart();

  //print data loaded from EEPROM

}

void Settings::SetDateTimeToRTC(DateTime dateTime)
{
  rtc->setManualTime(dateTime.year(), dateTime.month(), dateTime.day(), dateTime.hour(), dateTime.minute(), dateTime.second(), dateTime.dayOfTheWeek());
}

void Settings::setSSID(const char* ssid) {
  // Usa memcpy per copiare esattamente 32 byte nella variabile ssid
  // Se la stringa passata è più lunga di 31 caratteri, sarà troncata
  memset(config.ssid, 0, sizeof(config.ssid));  // Pulizia preventiva
  memcpy(config.ssid, ssid, min(strlen(ssid), sizeof(config.ssid) - 1));  // Copia limitata
  Serial.println("Set SSID");
  Serial.println(config.ssid);
}

void Settings::setPassword(const char* password) {
  // Usa memcpy per copiare esattamente 32 byte nella variabile password
  memset(config.password, 0, sizeof(config.password));  // Pulizia preventiva
  memcpy(config.password, password, min(strlen(password), sizeof(config.password) - 1));  // Copia limitata
  Serial.println("Set Password");
  Serial.println(config.password);
}

void Settings::setUseInternetTime(bool useInternetTime) {
  config.useInternetTime = useInternetTime;
  Serial.println("Set Use Internet Time");
  Serial.println(config.useInternetTime);
}

void Settings::setUseSTA(bool useSTA)
{
    config.useSTA = useSTA;
    Serial.println("Set Use STA");
    Serial.println(config.useSTA);
}

bool Settings::setDimTimes(int startDimTime, int endDimTime)
{
  //check if the start time is greater than the end time
  if (startDimTime <= 24 && endDimTime <= 24 && startDimTime >= 0 && endDimTime >= 0)
  {
      config.startDimTime = startDimTime;
      config.endDimTime = endDimTime;
      return true;
    
  }
  else
  {
    Serial.println("Time must be between 0 and 24");
    return false;
  }
  return false;
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

void Settings::printSettings()
{
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

}