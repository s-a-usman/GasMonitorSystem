// include/power_management.h
#ifndef POWER_MANAGEMENT_H
#define POWER_MANAGEMENT_H

#include <Arduino.h>

class PowerManagement {
public:
    void begin();
    void monitor();
private:
    const int BATTERY_PIN = 35;
    const float BATTERY_THRESHOLD = 3.2;
    float readBatteryVoltage();
};

#endif // POWER_MANAGEMENT_H