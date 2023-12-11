#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
//#include <ESPAsyncTCP.h>
AsyncWebServer server(80);

void OTAInit()
{
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/plain", "Ciao sono l'orologio di deddy"); });
 
  AsyncElegantOTA.begin(&server); // Start ElegantOTA
  server.begin();
  Serial.println("HTTP server started");
}