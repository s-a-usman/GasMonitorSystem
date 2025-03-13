#include "webserver.h"

SensorData ESPWebServer::currentData;

void ESPWebServer::begin() {
    // Initialize SPIFFS with delay to ensure proper mounting
    if (!SPIFFS.begin()) {
        Serial.println("An error occurred while mounting SPIFFS");
        if (!SPIFFS.begin()) { // Try again after delay
            Serial.println("SPIFFS mount failed again, continuing without it");
        } else {
            Serial.println("SPIFFS mounted successfully after retry");
        }
    } else {
        Serial.println("SPIFFS mounted successfully");
    }

    // Serve static files
    server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");

    // API endpoint for real-time data
    server.on("/api/data", HTTP_GET, [](AsyncWebServerRequest *request) { 
        DynamicJsonDocument doc(1024);
        doc["gas_level"] = currentData.gasLevel;
        doc["temperature"] = currentData.temperature;
        doc["vibration_x"] = currentData.vibrationX;
        doc["vibration_y"] = currentData.vibrationY;
        doc["vibration_z"] = currentData.vibrationZ;
        // doc["battery_voltage"] = currentData.batteryVoltage;

        String response;
        serializeJson(doc, response);
        request->send(200, "application/json", response);
    });

    // API endpoint for historical data
    server.on("/api/historical-data", HTTP_GET, [this](AsyncWebServerRequest *request) {
        this->handleHistoricalData(request);
    });


    // Captive portal DNS redirect handler
    server.onNotFound([](AsyncWebServerRequest *request) {
        request->redirect("http://gasmonitor.local/");
    });

    // Add a route to handle the Apple captive portal detection
    server.on("/hotspot-detect.html", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->redirect("http://gasmonitor.local/");
    });

    // Handle Android captive portal detection
    server.on("/generate_204", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->redirect("http://gasmonitor.local/");
    });

    // Handle Microsoft captive portal detection
    server.on("/ncsi.txt", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->redirect("http://gasmonitor.local/");
    });

    server.begin();
    Serial.println("Web server started");
}

void ESPWebServer::updateData(const SensorData& data) {
    currentData = data;
}

void ESPWebServer::handleHistoricalData(AsyncWebServerRequest *request) {
    String range = "1h"; // Default to 1 hour
    if (request->hasParam("range")) {
        range = request->getParam("range")->value();
    }

    DynamicJsonDocument doc(16384); // Larger document for historical data
    JsonArray array = doc.to<JsonArray>();

    if (!SD.begin()) {
        request->send(500, "application/json", "{\"error\": \"SD card initialization failed\"}");
        return;
    }

    readHistoricalData(range, array);

    String response;
    serializeJson(doc, response);
    request->send(200, "application/json", response);
}

unsigned long ESPWebServer::getTimeThreshold(const String& range) {
    unsigned long currentTime = millis();
    unsigned long threshold;
    
    if (range == "1h") {
        threshold = currentTime - (60UL * 60 * 1000); // 1 hour in milliseconds
    } else if (range == "24h") {
        threshold = currentTime - (24UL * 60 * 60 * 1000); // 24 hours in milliseconds
    } else if (range == "1m") {
        threshold = currentTime - (30UL * 24 * 60 * 60 * 1000); // ~30 days in milliseconds
    } else {
        threshold = currentTime - (60UL * 60 * 1000); // Default to 1 hour
    }
    
    return threshold;
}

void ESPWebServer::readHistoricalData(const String& range, JsonArray& array) {
    if (!SD.exists("/log.csv")) {
        Serial.println("Log file not found");
        return;
    }

    File file = SD.open("/log.csv", FILE_READ);
    if (!file) {
        Serial.println("Failed to open log file");
        return;
    }

    // Skip header line
    file.readStringUntil('\n');

    // Set a default sampling interval based on range
    int sampleInterval;
    if (range == "1h") {
        sampleInterval = 1; // Every record (assuming 1 record per second)
    } else if (range == "24h") {
        sampleInterval = 60; // Every 60th record (1 per minute)
    } else if (range == "1m") {
        sampleInterval = 1440; // Every 1440th record (1 per hour)
    } else {
        sampleInterval = 1;
    }

    // Read all available data
    std::vector<String> validLines;
    String line;
    
    while (file.available()) {
        line = file.readStringUntil('\n');
        if (line.length() == 0) continue;
        validLines.push_back(line);
    }
    
    // Process data with appropriate sampling
    // If we have very little data, use all of it
    if (validLines.size() < 60) { // If less than 60 data points, show everything
        sampleInterval = 1;
    }
    
    for (size_t i = 0; i < validLines.size(); i += sampleInterval) {
        line = validLines[i];
        
        // Parse the line
        int commaPos = line.indexOf(",");
        unsigned long timestamp = line.substring(0, commaPos).toInt();
        line = line.substring(commaPos + 1);
        
        commaPos = line.indexOf(",");
        float gas = line.substring(0, commaPos).toFloat();
        line = line.substring(commaPos + 1);
        
        commaPos = line.indexOf(",");
        float temp = line.substring(0, commaPos).toFloat();
        line = line.substring(commaPos + 1);
        
        commaPos = line.indexOf(",");
        float vibX = line.substring(0, commaPos).toFloat();
        line = line.substring(commaPos + 1);
        
        commaPos = line.indexOf(",");
        float vibY = line.substring(0, commaPos).toFloat();
        line = line.substring(commaPos + 1);
        
        commaPos = line.indexOf(",");
        float vibZ = line.substring(0, commaPos).toFloat();
        // float batteryVoltage = line.substring(commaPos + 1).toFloat();

        // Create JSON object for this data point
        JsonObject dataPoint = array.createNestedObject();
        dataPoint["timestamp"] = timestamp;
        dataPoint["gas_level"] = gas;
        dataPoint["temperature"] = temp;
        dataPoint["vibration_x"] = vibX;
        dataPoint["vibration_y"] = vibY;
        dataPoint["vibration_z"] = vibZ;
        // dataPoint["battery_voltage"] = batteryVoltage;
    }

    file.close();
    
    // Debug information
    Serial.print("Historical data points loaded: ");
    Serial.println(array.size());
}