//webserver.h
#ifndef ESPWEBSERVER_H
#define ESPWEBSERVER_H

#include <Arduino.h>
#include <FS.h>
#include <SPIFFS.h>
#include <WebServer.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <SD.h>
#include "sensors.h"

class ESPWebServer {
public:
    void begin();
    void updateData(const SensorData& data);

private:
    AsyncWebServer server{80};
    static SensorData currentData;
    void handleHistoricalData(AsyncWebServerRequest *request);
    unsigned long getTimeThreshold(const String& range);
    void readHistoricalData(const String& range, JsonArray& array);
    
};

#endif // ESPWEBSERVER_H
