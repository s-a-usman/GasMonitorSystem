#ifndef CONFIG_H
#define CONFIG_H

// Pin definitions
#define MQ5_PIN 34
#define TEMP_PIN 4
#define BUZZER_PIN 2
#define SD_CS_PIN 5
#define BATTERY_PIN 35

// Thresholds
#define GAS_THRESHOLD 500.0f
#define TEMP_THRESHOLD 50.0f
#define VIBRATION_THRESHOLD 2.0f
#define BATTERY_THRESHOLD 3.2f

// Sampling intervals
#define SENSOR_SAMPLING_MS 1000
#define WEB_UPDATE_MS 1000
#define LOG_INTERVAL_MS 5000

#endif // CONFIG_H