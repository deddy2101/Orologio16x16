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
  rtc.init();
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

unsigned long previousMillis = 0;    // Memorizza l'ultimo tempo in cui hai cambiato la visualizzazione
const long intervalTime = 20000;     // Intervallo per la visualizzazione dell'ora (20 secondi)
const long intervalDate = 10000;     // Intervallo per la visualizzazione della data (10 secondi)
bool showTime = true; 

void loop() {
  unsigned long currentMillis = millis();  // Ottieni il tempo corrente
  
  // Controlla se è tempo di cambiare visualizzazione
  if (showTime && currentMillis - previousMillis >= intervalTime) {
    previousMillis = currentMillis;  // Aggiorna il tempo dell'ultimo cambiamento
    // Cambia a visualizzazione data
    DateTime now = rtc.getCurrentTime();
    int day = now.day();
    int month = now.month();
    int dayOfWeek = now.dayOfTheWeek();
    int datetime[3] = {day, month, dayOfWeek};
    display.displayDate(datetime);  // Mostra la data
    showTime = false;  // Cambia alla visualizzazione della data
  }
  else if (!showTime && currentMillis - previousMillis >= intervalDate) {
    previousMillis = currentMillis;  // Aggiorna il tempo dell'ultimo cambiamento
    // Cambia a visualizzazione ora
    DateTime now = rtc.getCurrentTime();
    display.displayTime(now.hour(), now.minute(), true);  // Mostra l'ora
    showTime = true;  // Cambia alla visualizzazione dell'ora
  }

}
