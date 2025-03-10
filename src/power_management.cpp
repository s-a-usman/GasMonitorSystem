// src/power_management.cpp
#include "power_management.h"
#include <Arduino.h>

void PowerManagement::begin() {
    pinMode(BATTERY_PIN, INPUT);
}

void PowerManagement::monitor() {
    float voltage = readBatteryVoltage();
    if (voltage < BATTERY_THRESHOLD) {
        Serial.println("Low battery warning!");
    }
}

float PowerManagement::readBatteryVoltage() {
    int raw = analogRead(BATTERY_PIN);
    // Convert ADC reading to voltage (adjust these values based on your voltage divider)
    return (raw * 3.3 / 4095.0) * 2; // Assuming a 1:1 voltage divider
}