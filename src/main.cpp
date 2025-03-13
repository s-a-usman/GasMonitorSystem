/* This is the main file for the project. It includes the necessary libraries and initializes the global objects. 
It also contains the setup and loop functions that will be executed by the ESP32 microcontroller. 
The setup function initializes the components and connects to Wi-Fi, 
while the loop function reads sensor data, checks for alerts, logs data, updates the web interface, and monitors power. 
The loop function also contains a delay of 1 second to control the frequency of sensor readings and updates.
*/
// src/main.cpp
#include <Arduino.h>
#include "ESPAsyncWebServer.h"
#include <SPIFFS.h>
#include "SD.h"  // Keep only this one
#include "webserver.h"
#include "datalogger.h"
#include "sensors.h"
#include "power_management.h"
#include <WiFi.h>
#include <DNSServer.h>
#include <ESPmDNS.h>  // Added for mDNS support
// #include "config.h"

// Global objects
Sensors sensors;
ESPWebServer webServer;
DataLogger dataLogger;
PowerManagement powerMgmt;
DNSServer dnsServer;

// Access Point settings
const char* ap_ssid = "GasMonitor";  // Name of the WiFi network
const char* ap_password = "gasmonitor123";  // Password for the WiFi network
IPAddress apIP(192, 168, 4, 1);  // IP address for the AP
const byte DNS_PORT = 53;  // DNS server port

unsigned long lastHeartbeat = 0;

void scanI2C() {
    Serial.println("Scanning I2C...");
    for (byte address = 1; address < 127; address++) {
        Wire.beginTransmission(address);
        if (Wire.endTransmission() == 0) {
            Serial.print("Device found at 0x");
            Serial.println(address, HEX);
        }
    }
}

// Captive portal DNS handler
class CaptiveRequestHandler : public AsyncWebHandler {
    public:
        CaptiveRequestHandler() {}
        virtual ~CaptiveRequestHandler() {}
    
        bool canHandle(AsyncWebServerRequest *request) {
            // Handle all requests that aren't to the known web server
            return true;
        }
    
        void handleRequest(AsyncWebServerRequest *request) {
            // Redirect all requests to the captive portal
            request->redirect("http://192.168.4.1/");
        }
};


void setup() {
    Serial.begin(115200);
    Wire.begin();
    scanI2C();

    // Set up the ESP32 as an access point
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    WiFi.softAP(ap_ssid, ap_password);
    
    Serial.println("Access Point Started");
    Serial.print("AP IP address: ");
    Serial.println(apIP);
    Serial.print("SSID: ");
    Serial.println(ap_ssid);
    Serial.print("Password: ");
    Serial.println(ap_password);
    
    // Start DNS server for captive portal
    dnsServer.start(DNS_PORT, "*", apIP);
    
    // Set up mDNS responder
    if (!MDNS.begin("gasmonitor")) {
        Serial.println("Error setting up mDNS responder!");
    } else {
        Serial.println("mDNS responder started");
        Serial.println("You can now access the web interface at http://gasmonitor.local");
    }

    // Initialize other components
    sensors.begin();
    webServer.begin();
    dataLogger.begin();
    powerMgmt.begin();

    digitalWrite(2, HIGH);  // Turn on built-in LED to indicate AP is running
}


void loop() {
    // Process DNS requests
    dnsServer.processNextRequest();
    
    // Read sensor data
    SensorData data = sensors.readAll();
    
    // Check for alerts
    sensors.checkAlerts(data);
    
    // Log data
    dataLogger.logData(data);
    
    // Update web interface
    webServer.updateData(data);
    
    // Monitor power
    //powerMgmt.monitor();
    
    static unsigned long lastUpdate = 0;
    if (millis() - lastUpdate > 1000) {
        lastUpdate = millis();
        // Execute sensor reading, logging, and updates
    }
    
    // Heartbeat check
    if (millis() - lastHeartbeat > 30000) { // 30 seconds
        lastHeartbeat = millis();
        Serial.println("Heartbeat: System running");
        Serial.print("Free heap: ");
        Serial.println(ESP.getFreeHeap());
        
        // Print number of connected clients
        Serial.print("Connected clients: ");
        Serial.println(WiFi.softAPgetStationNum());
    }
}