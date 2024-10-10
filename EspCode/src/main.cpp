#include "DisplayManager/DisplayManager.h"
#include "WiFiManager/WiFiManager.h"
#include "TimeManager/TimeManager.h"
#include "Settings/Settings.h"
#include "WebServerManager/WebServerManager.h"
#include "RTCManager/RTCManager.h"

RTCManager rtc;  // Crea l'oggetto RTCManager
Settings settings(&rtc);  // Crea l'oggetto Settings
DisplayManager display(256, 150);
WiFiManager wifi("SSID", "PASSWORD");
TimeManager timeManager(0);
WebServerManager webServer(&settings);  // Crea l'oggetto WebServerManager

void setup() {
  Serial.begin(9600);
  settings.loadSettings();
  display.initDisplay();
  wifi.initWIFI(settings.getUseSTA());
  timeManager.init(settings.getUseInternetTime());
  if(settings.getUseInternetTime())  
  {
    int datetime[8];
    timeManager.getDateTime(datetime);
    rtc.setTimeFromNTP(datetime);
    rtc.setDayOfWeek(datetime[7]);
  }
  webServer.initServer();
}

void loop() {
  DateTime now = rtc.getCurrentTime();
  int day = now.day();
  int month = now.month();
  int dayOfWeek = now.dayOfTheWeek();
  int datetime[3] = {day, month,dayOfWeek};
  display.displayTime(now.hour(), now.minute(), true);
  
  display.displayDate(datetime);
  delay(1000);
}
