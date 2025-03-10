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
// #include "config.h"

// Global objects
Sensors sensors;
ESPWebServer webServer;
DataLogger dataLogger;
PowerManagement powerMgmt;


const char* ssid = "Redmi";
const char* password = "0000001111";

#include <Wire.h>

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


void setup() {
    Serial.begin(115200);
    Wire.begin();
    scanI2C();

    // Connect to Wi-Fi
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected");
    digitalWrite(2, HIGH);
    Serial.print("ESP32 IP Address: ");
    Serial.println(WiFi.localIP());

    // Initialize other components
    sensors.begin();
    webServer.begin();
    dataLogger.begin();
    powerMgmt.begin();
}


void loop() {
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
}    