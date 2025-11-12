#include "DisplayManager/DisplayManager.h"
#include "WiFiManager/WiFiManager.h"
#include "TimeManager/TimeManager.h"
#include "Settings/Settings.h"
#include "WebServerManager/WebServerManager.h"
#include "RTCManager/RTCManager.h"
#include "WebSocketManager/WebSocketManager.h"
#include "SnakeGame/SnakeGame.h"
#include "PingPongGame/PingPongGame.h"

// Enum per le modalità operative
enum OperationMode {
  MODE_CLOCK,
  MODE_SNAKE,
  MODE_PONG
};

RTCManager rtc;
DisplayManager display(256, 150);
Settings settings(&rtc, &display);
WiFiManager wifi(&settings);
TimeManager timeManager(0);
WebServerManager webServer(&settings);
WebSocketManager wsManager;
SnakeGame snakeGame(&display, &wsManager);
PingPongGame pongGame(&display, &wsManager);

OperationMode currentMode = MODE_CLOCK;

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
  
  // Inizializza il WebSocket Manager DOPO il web server
  wsManager.init(&webServer.server);
  
  // Inizializza i giochi
  snakeGame.init();
  pongGame.init();
  
  display.scrollTextFull("Deddys clock", CRGB::Red);
  
  // Abilita l'effetto neve
  display.enableSnow(settings.getUseSnow());
  
  //print the time
  DateTime now = rtc.getCurrentTime();
  Serial.print(now.year());
  Serial.print("-");
  Serial.print(now.month());
  Serial.print("-");
  Serial.print(now.day());
  Serial.print(" ");
  Serial.print(now.hour());
  Serial.print(":");
  Serial.print(now.minute());
  Serial.print(":");
  Serial.println(now.second());

  Serial.println("Setup completato!");
  Serial.println("Invia 'START_SNAKE' o 'START_PONG' via WebSocket per giocare");
}

unsigned long previousMillis = 0;
const long intervalTime = 20000;
const long intervalDate = 10000;
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

void runClockMode() {
  // Aggiorna la posizione dei fiocchi di neve
  display.enableSnow(settings.getUseSnow());
  display.updateSnow();
  
  unsigned long currentMillis = millis();
  
  DateTime now = rtc.getCurrentTime();
  int hour = now.hour();
  int startDimTime, endDimTime;
  settings.getDimTimes(&startDimTime, &endDimTime);
  
  bool isNight = (hour >= startDimTime || hour < endDimTime);

  if (isNight) {
    // Durante la notte, mostra solo l'ora
    if (currentMillis - previousMillis >= intervalTime) {
      checkIfHasToBeDimmed();
      previousMillis = currentMillis;
      display.displayNigntTime(now.hour(), now.minute());
      display.saveBaseDisplay();
      display.applySnowOverlay();
      FastLED.show();
    }
  } else {
    // Durante il giorno, alterna visualizzazione tra data e ora
    if (showTime && currentMillis - previousMillis >= intervalTime) {
      checkIfHasToBeDimmed();
      previousMillis = currentMillis;
      int day = now.day();
      int month = now.month();
      int dayOfWeek = now.dayOfTheWeek();
      int datetime[3] = {day, month, dayOfWeek};
      display.displayDate(datetime);
      display.saveBaseDisplay();
      display.applySnowOverlay();
      FastLED.show();
      showTime = false;
      
    } 
    else if (!showTime && currentMillis - previousMillis >= intervalDate) {
      previousMillis = currentMillis;
      display.displayTime(now.hour(), now.minute(), true);
      display.saveBaseDisplay();
      display.applySnowOverlay();
      FastLED.show();
      showTime = true;
    }
  }
  
  // Aggiorna la visualizzazione della neve continuamente
  static unsigned long lastSnowDisplay = 0;
  if (millis() - lastSnowDisplay >= 200) {
    lastSnowDisplay = millis();
    display.applySnowOverlay();
    FastLED.show();
  }
}

void loop() {
  // Controlla i comandi WebSocket
  if (wsManager.hasCommand()) {
    GameInput input = wsManager.getInput();
    
    // Se riceviamo START_SNAKE, passiamo alla modalità Snake
    if (input.command == CMD_START_SNAKE) {
      currentMode = MODE_SNAKE;
      display.enableSnow(false); // Disabilita la neve durante il gioco
      snakeGame.handleCommand(input.command);
      Serial.println("Switched to SNAKE mode");
    }
    // Se riceviamo START_PONG, passiamo alla modalità Ping Pong
    else if (input.command == CMD_START_PONG) {
      currentMode = MODE_PONG;
      display.enableSnow(false); // Disabilita la neve durante il gioco
      pongGame.handleCommand(input.command, 0);
      Serial.println("Switched to PONG mode");
    }
    // Se riceviamo STOP_GAME, torniamo alla modalità orologio
    else if (input.command == CMD_STOP_GAME) {
      if (currentMode == MODE_SNAKE) {
        snakeGame.handleCommand(input.command);
      } else if (currentMode == MODE_PONG) {
        pongGame.handleCommand(input.command, 0);
      }
      currentMode = MODE_CLOCK;
      display.enableSnow(settings.getUseSnow()); // Riabilita la neve
      Serial.println("Switched to CLOCK mode");
    }
    // Altri comandi vanno al gioco appropriato
    else {
      if (currentMode == MODE_SNAKE) {
        snakeGame.handleCommand(input.command);
      } else if (currentMode == MODE_PONG) {
        // Per Ping Pong, passa anche il numero del giocatore
        pongGame.handleCommand(input.command, input.player);
      }
    }
  }
  
  // Esegui la logica appropriata in base alla modalità
  if (currentMode == MODE_CLOCK) {
    runClockMode();
  } 
  else if (currentMode == MODE_SNAKE) {
    snakeGame.update();
    
    // Se il gioco è finito, torna automaticamente alla modalità orologio dopo 3 secondi
    if (snakeGame.getState() == GAME_OVER) {
      static unsigned long gameOverTime = 0;
      if (gameOverTime == 0) {
        gameOverTime = millis();
      }
      
      if (millis() - gameOverTime > 3000) {
        currentMode = MODE_CLOCK;
        display.enableSnow(settings.getUseSnow());
        gameOverTime = 0;
        Serial.println("Auto-switched to CLOCK mode after game over");
      }
    }
  }
  else if (currentMode == MODE_PONG) {
    pongGame.update();
    
    // Se il gioco è finito, torna automaticamente alla modalità orologio dopo 3 secondi
    if (pongGame.getState() == PONG_GAME_OVER) {
      static unsigned long pongGameOverTime = 0;
      if (pongGameOverTime == 0) {
        pongGameOverTime = millis();
      }
      
      if (millis() - pongGameOverTime > 3000) {
        currentMode = MODE_CLOCK;
        display.enableSnow(settings.getUseSnow());
        pongGameOverTime = 0;
        Serial.println("Auto-switched to CLOCK mode after pong game over");
      }
    }
  }
}