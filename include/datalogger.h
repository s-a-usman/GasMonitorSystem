//src/datalogger.h
#ifndef DATALOGGER_H
#define DATALOGGER_H

#include <Arduino.h>
#include "sensors.h"
#include <SPI.h>
#include "SD.h"  // Use ESP32's SD library
#include "FS.h"

class DataLogger {
public:
    void begin();
    void logData(const SensorData& data);
private:
    const int CS_PIN = 5;
    String createLogEntry(const SensorData& data);
};

#endif // DATALOGGER_H