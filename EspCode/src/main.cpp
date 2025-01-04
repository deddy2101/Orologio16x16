#include "DisplayManager/DisplayManager.h"
#include "WiFiManager/WiFiManager.h"
#include "TimeManager/TimeManager.h"
#include "Settings/Settings.h"
#include "WebServerManager/WebServerManager.h"
#include "RTCManager/RTCManager.h"

RTCManager rtc;  // Crea l'oggetto RTCManager
DisplayManager display(256, 150);
Settings settings(&rtc, &display);  // Crea l'oggetto Settings
WiFiManager wifi(&settings);
TimeManager timeManager(0);
WebServerManager webServer(&settings);  // Crea l'oggetto WebServerManager

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.begin(115200);
  delay(2000);
  Serial.println("Inizializzazione...");
  settings.loadSettings();
  rtc.init();
  display.initDisplay();
  wifi.initWIFI(settings.getUseSTA());
  
  if(settings.getUseInternetTime() && WiFi.status() == WL_CONNECTED)
  {
    timeManager.init(settings.getUseInternetTime());
    int datetime[8];
    timeManager.getDateTime(datetime);

    rtc.setTimeFromNTP(datetime);
    rtc.setDayOfWeek(datetime[7]);
  } 
  webServer.initServer();
  display.scrollTextFull(wifi.getLocalIP(), CRGB::Red);
}

unsigned long previousMillis = 0;    // Memorizza l'ultimo tempo in cui hai cambiato la visualizzazione
const long intervalTime = 20000;     // Intervallo per la visualizzazione dell'ora (20 secondi)
const long intervalDate = 10000;     // Intervallo per la visualizzazione della data (10 secondi)
bool showTime = true; 

void checkIfHasToBeDimmed()
{
  DateTime now = rtc.getCurrentTime();
  int hour = now.hour();
  int startDimTime, endDimTime;
  settings.getDimTimes(&startDimTime, &endDimTime);
  if(hour >= startDimTime|| hour < endDimTime)
  {
    int nightDim, dayDim;
    settings.getDimValues(&nightDim, &dayDim);
    display.setMaxBrightness(nightDim);
  }
  else
  {
    int nightDim, dayDim;
    settings.getDimValues(&nightDim, &dayDim);
    display.setMaxBrightness(dayDim);
  }
}

void loop() {
  unsigned long currentMillis = millis();  // Ottieni il tempo corrente
  
  // Ottieni l'ora corrente e verifica se è notte
  DateTime now = rtc.getCurrentTime();
  int hour = now.hour();
  int startDimTime, endDimTime;
  settings.getDimTimes(&startDimTime, &endDimTime);
  
  bool isNight = (hour >= startDimTime || hour < endDimTime);

  if (isNight) {
    // Durante la notte, mostra solo l'ora
    if (currentMillis - previousMillis >= intervalTime) {
      checkIfHasToBeDimmed(); // Regola la luminosità
      previousMillis = currentMillis;  // Aggiorna il tempo dell'ultimo cambiamento
      display.displayTime(now.hour(), now.minute(), true);  // Mostra solo l'ora
    }
  } else {
    // Durante il giorno, alterna visualizzazione tra data e ora
    if (showTime && currentMillis - previousMillis >= intervalTime) {
      checkIfHasToBeDimmed(); // Regola la luminosità
      previousMillis = currentMillis;  // Aggiorna il tempo dell'ultimo cambiamento
      // Cambia a visualizzazione data
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
      display.displayTime(now.hour(), now.minute(), true);  // Mostra l'ora
      showTime = true;  // Cambia alla visualizzazione dell'ora
    }
  }
}
