#ifndef WEBSERVERMANAGER_H
#define WEBSERVERMANAGER_H

#include <ESPAsyncWebServer.h>
//#include <AsyncElegantOTA.h>
#include "Settings/Settings.h"
#include "SPIFFS.h"


class WebServerManager {
  private:
    AsyncWebServer server;
    Settings* settings;

  public:
    // Constructor
    WebServerManager(Settings* settings);

    // Initialize the web server and OTA
    void initServer();
};

#endif
