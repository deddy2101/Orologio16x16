#include <WiFi.h>

const char *ssid = "WI-FI_2018"; //MaddoNS
const char *password = "S74s8jbvie74h"; //deddyebello2022
//const char *ssid = "Wind3 HUB - E75186";
//const char *password = "6tcpkdz76b9bhtkd";

IPAddress local_IP(192, 168, 1, 151); //214 relay //215 button //216 button
IPAddress gateway(192, 168, 1, 254);
//IPAddress gateway(192, 168, 1, 254);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(1, 1, 1, 1);   // optional
IPAddress secondaryDNS(1, 0, 0, 1); // optional

bool initWIFI()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.println("");
    // Configures static IP address
    if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS))
    {
        Serial.println("STA Failed to configure");
        return false;
    }
    // Wait for connection
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(":)-");

    }
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    return true;
}