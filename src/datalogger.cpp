//src/datalogger.cpp
#include "datalogger.h"

void DataLogger::begin() {
    SPI.begin();
    if (!SD.begin(CS_PIN)) {
        Serial.println("SD Card initialization failed!");
        return;
    }
    Serial.println("SD Card initialized.");
    
    // Create header if file doesn't exist
    File file = SD.open("/log.csv", FILE_WRITE);
    if (file) {
        file.println("timestamp,gas,temperature,vibration_x,vibration_y,vibration_z");
        file.close();
    }
}

void DataLogger::logData(const SensorData& data) {
    File file = SD.open("/log.csv", FILE_APPEND);
    if (file) {
        String logEntry = createLogEntry(data);
        file.println(logEntry);
        file.close();
    } else {
        Serial.println("Failed to open log file");
    }
}

String DataLogger::createLogEntry(const SensorData& data) {
    String timestamp = String(millis());
    return timestamp + "," +
           String(data.gasLevel) + "," +
           String(data.temperature) + "," +
           String(data.vibrationX) + "," +
           String(data.vibrationY) + "," +
           String(data.vibrationZ); + "," +
           String(data.batteryVoltage);
}