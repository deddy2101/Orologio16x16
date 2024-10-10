#include "WebServerManager.h"

// Constructor
WebServerManager::WebServerManager(Settings* settings) : server(80), settings(settings) {
    if (!SPIFFS.begin(true)) {  // Monta il filesystem SPIFFS
    Serial.println("An error has occurred while mounting SPIFFS");
    return;
  }
  Serial.println("SPIFFS mounted successfully");
}

// Initialize the web server and OTA
void WebServerManager::initServer() {
  server.on("/", HTTP_GET, [this](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/index.html", "text/html");
  });

  // Servire la cartella assets (CSS e JS)
  server.serveStatic("/assets", SPIFFS, "/assets");
  // Endpoint to get WiFi settings
  server.on("/getWifiSettings", HTTP_GET, [this](AsyncWebServerRequest *request) {
    String message = "SSID: " + String(settings->getSSID()) + 
                     "\nPassword: " + String(settings->getPassword());
    request->send(200, "text/plain", message);
  });

  // Endpoint to set WiFi settings (POST)
  server.on("/setWifiSettings", HTTP_POST, [this](AsyncWebServerRequest *request) {
    if (request->hasParam("ssid", true) && request->hasParam("password", true)) {
      String ssid = request->getParam("ssid", true)->value();
      String password = request->getParam("password", true)->value();
      settings->setSSID(ssid.c_str());
      settings->setPassword(password.c_str());
      settings->saveSettings();
      request->send(200, "text/plain", "WiFi settings updated");
    } else {
      request->send(400, "text/plain", "Missing parameters");
    }
  });

  // Endpoint to get time setting (GET)
  server.on("/getTimeSetting", HTTP_GET, [this](AsyncWebServerRequest *request) {
    String message = "Use internet time: " + String(settings->getUseInternetTime() ? "true" : "false");
    request->send(200, "text/plain", message);
  });

  // Endpoint to set time setting (POST)
  server.on("/setInternetTime", HTTP_POST, [this](AsyncWebServerRequest *request) {
    if (request->hasParam("useInternetTime", true)) {
      String useInternetTimeStr = request->getParam("useInternetTime", true)->value();
      bool useInternetTime = (useInternetTimeStr == "true");
      settings->setUseInternetTime(useInternetTime);
      settings->saveSettings();
      request->send(200, "text/plain", "Time setting updated");
    } else {
      request->send(400, "text/plain", "Missing parameter");
    }
  });

  server.on("/setDateTime", HTTP_POST, [this](AsyncWebServerRequest *request) {
    // get hour minute secons day month year
    if (request->hasParam("hour", true) && request->hasParam("minute", true) && request->hasParam("second", true) &&
        request->hasParam("day", true) && request->hasParam("month", true) && request->hasParam("year", true)) {
      int hour = request->getParam("hour", true)->value().toInt();
      int minute = request->getParam("minute", true)->value().toInt();
      int second = request->getParam("second", true)->value().toInt();
      int day = request->getParam("day", true)->value().toInt();
      int month = request->getParam("month", true)->value().toInt();
      int year = request->getParam("year", true)->value().toInt();
      DateTime dateTime = DateTime(year, month, day, hour, minute, second);
      settings->SetDateTimeToRTC(dateTime);
      request->send(200, "text/plain", "Date and time updated");
    } else {
      request->send(400, "text/plain", "Missing parameters");
    }
  });

  // OTA support
  //AsyncElegantOTA.begin(&server);  // Start ElegantOTA

  // Start the server
  server.begin();
  Serial.println("HTTP server started");
}
