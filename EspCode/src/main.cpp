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
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.begin(115200);
  delay(2000);
  Serial.println("Inizializzazione...");
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
  display.scrollText(wifi.getLocalIP(), CRGB::Green);
  display.scrollTextFull("Hello World 123.", CRGB::Red);
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
    display.setMaxBrightness(10);
  }
  else
  {
    display.setMaxBrightness(150);
  }
}

void loop() {
  

  unsigned long currentMillis = millis();  // Ottieni il tempo corrente
  
  // Controlla se è tempo di cambiare visualizzazione
  if (showTime && currentMillis - previousMillis >= intervalTime) {
    checkIfHasToBeDimmed();
    previousMillis = currentMillis;  // Aggiorna il tempo dell'ultimo cambiamento
    // Cambia a visualizzazione data
    DateTime now = rtc.getCurrentTime();
    //printout all the values
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.print(" ");
    Serial.print(now.dayOfTheWeek(), DEC);
    Serial.println();

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
