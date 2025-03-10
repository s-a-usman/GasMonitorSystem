// include/sensors.h
#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <DallasTemperature.h>
#include <OneWire.h>

struct SensorData {
    float gasLevel;
    float temperature;
    float vibrationX;
    float vibrationY;
    float vibrationZ;
    float batteryVoltage;
};

class Sensors {
public:
    void begin();
    SensorData readAll();
    void checkAlerts(const SensorData& data);
    
private:
    // const float GAS_OFFSET = 3500.0;  // OFFSET for MQ5 sensor
    // Const float GAS = ;
    const float GAS_THRESHOLD = 4500.0;  // 3,300 reading in fresh air, set for 3,500 for alerts
    const float TEMP_THRESHOLD = 40.0;
    const float VIBRATION_THRESHOLD = 0.98; //0.9 to 1g means normal readings = 9.81 m/s^2
    
    const int MQ5_PIN = 34;
    const int TEMP_PIN = 4;
    const int BUZZER_PIN = 13; //13; 
    
    Adafruit_MPU6050 mpu;
    OneWire oneWire = OneWire(TEMP_PIN);
    DallasTemperature tempSensor;
};

#endif // SENSORS_H