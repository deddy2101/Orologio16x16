#include "WiFiManager.h"

WiFiManager::WiFiManager(const char* ssid, const char* password)
    : ssid(ssid), password(password) {}

bool WiFiManager::initWIFI(bool useSTA) {
    if (useSTA) {
        // Modalità WiFi Station (STA)
        WiFi.mode(WIFI_STA);  // Imposta la modalità STA (client WiFi)
        WiFi.begin(ssid, password);  // Connetti alla rete WiFi con SSID e password

        Serial.println("Attempting to connect to WiFi using DHCP...");

        int maxRetries = 20;
        int retries = 0;

        // Attendere la connessione
        while (WiFi.status() != WL_CONNECTED && retries < maxRetries) {
            delay(500);
            Serial.print(".");
            retries++;
        }

        Serial.println("");

        if (WiFi.status() == WL_CONNECTED) {
            // Connessione riuscita
            Serial.print("Connected to ");
            Serial.println(ssid);
            Serial.print("IP address assigned by DHCP: ");
            Serial.println(WiFi.localIP());
            return true;
        } else {
            // Connessione fallita
            Serial.println("Failed to connect to WiFi");
            return false;
        }
    } else {
        // Modalità Access Point (AP)
        WiFi.mode(WIFI_AP);  // Imposta la modalità AP
        const char* apSSID = "MyAccessPoint";
        const char* apPassword = "";

        // Imposta l'indirizzo IP dell'AP (192.168.1.1)
        IPAddress local_IP(192, 168, 1, 1);
        IPAddress gateway(192, 168, 1, 1);
        IPAddress subnet(255, 255, 255 ,0);
        
        // Configura l'AP con l'IP statico
        if (!WiFi.softAPConfig(local_IP, gateway, subnet)) {
            Serial.println("Failed to configure Access Point IP.");
            return false;
        }

        Serial.println("Starting Access Point...");
        if (WiFi.softAP(apSSID, apPassword)) {
            Serial.println("Access Point started.");
            Serial.print("AP IP address: ");
            Serial.println(WiFi.softAPIP());
            return true;
        } else {
            Serial.println("Failed to start Access Point.");
            return false;
        }
    }
}
