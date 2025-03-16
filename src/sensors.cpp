#include "sensors.h"

void Sensors::begin() {
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(MQ5_PIN, INPUT);
    
    oneWire.begin(TEMP_PIN);
    tempSensor.setOneWire(&oneWire);
    tempSensor.begin();
    tempSensor.setResolution(12);  // Set temperature sensor resolution
    
    if (!mpu.begin(0x68)) {
        Serial.println("MPU6050 init failed on 0x68, trying 0x69...");
        if (!mpu.begin(0x69)) {
            Serial.println("MPU6050 init failed on both addresses");
        }
    }
    
    mpu.setAccelerometerRange(MPU6050_RANGE_4_G);

//         // Read initial offsets
//         sensors_event_t a, g, temp;
//         mpu.getEvent(&a, &g, &temp);
    
//         accelX_offset = a.acceleration.x;
//         accelY_offset = a.acceleration.y;
//         accelZ_offset = a.acceleration.z; // Capture gravity effect at rest
 }

SensorData Sensors::readAll() {
    SensorData data;
    
    // Read gas sensor
    data.gasLevel = analogRead(MQ5_PIN) / 10.0; // Adjusted formula to get 400 when reading 4000
    
    // Read temperature
    tempSensor.requestTemperatures();
    data.temperature = tempSensor.getTempCByIndex(0);
    
    // Read vibration
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    data.vibrationX = a.acceleration.x;
    data.vibrationY = a.acceleration.y;
    data.vibrationZ = a.acceleration.z;

    data.vibrationX /= 9.81;  // Convert to g
    data.vibrationY /= 9.81;  // Convert to g
    data.vibrationZ /= 9.81;  // Convert to g  

    Serial.print("Vibration X: ");
    Serial.println(data.vibrationX);
    Serial.print("Vibration Y: ");
    Serial.println(data.vibrationY);
    Serial.print("Vibration Z: ");
    Serial.println(data.vibrationZ);


    // Read battery voltage
    //data.batteryVoltage = analogRead(35) * 3.3 / 4095.0 * 2;  // Assuming voltage divider
    

    
    return data;
}

void Sensors::checkAlerts(const SensorData& data) {
    // static bool isBeeping = false;
    // static unsigned long lastBeepTime = 0;
    // static int beepCount = 0;
    static bool alertActive = false;

    // Check if an alert is triggered
    alertActive = false;

    Serial.print("Gas Level: ");
    Serial.println(analogRead(MQ5_PIN)/ 10);
    Serial.print("Temperature: ");
    Serial.println(data.temperature);
    
    if (data.gasLevel > GAS_THRESHOLD) {
        Serial.println("Gas Alert!");
        alertActive = true;
    }
    
    if (data.temperature > TEMP_THRESHOLD) {
        Serial.println("Temperature Alert!");
        alertActive = true;
    }
    
    float vibration = sqrt(pow(data.vibrationX, 2) + 
                          pow(data.vibrationY, 2) + 
                          pow(data.vibrationZ, 2));
    if (vibration > VIBRATION_THRESHOLD) {
        Serial.println("Vibration Alert!");
        alertActive = true;
    
    Serial.print("Vibration in G: ");   
    Serial.println(vibration);
    }

    // Buzzer logic with non-blocking delay
    // if (alertActive) {
    //     if (millis() - lastBeepTime >= (isBeeping ? 500 : 150)) {
    //         isBeeping = !isBeeping;  // Toggle buzzer state
    //         digitalWrite(BUZZER_PIN, isBeeping ? HIGH : LOW);
    //         lastBeepTime = millis();

    //         if (!isBeeping) { // Count beeps only when OFF
    //             beepCount++;
    //         }

    //         if (beepCount >= 5) { // Stop after 5 beeps (adjustable)
    //             beepCount = 0;
    //             alertActive = false;
    //             digitalWrite(BUZZER_PIN, LOW);
    //         }
    //     }
    // } else {
    //     digitalWrite(BUZZER_PIN, LOW);  // Turn off buzzer if no alert
    //     beepCount = 0;
    // }
    digitalWrite(BUZZER_PIN, alertActive ? HIGH : LOW);
}
