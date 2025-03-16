# Gas Monitor System

## Project Overview

The Gas Monitor System is an ESP32-based project designed to monitor gas levels, temperature, and vibration. It logs the sensor data, provides real-time alerts, and serves a web interface for data visualization and historical data access.

## Features

- **Real-time Monitoring**: Continuously monitors gas levels, temperature, and vibration.
- **Data Logging**: Logs sensor data to an SD card for historical analysis.
- **Web Interface**: Provides a web interface to view real-time data and historical data.
- **Alerts**: Generates alerts when sensor readings exceed predefined thresholds.

## System Requirements

- **Hardware**:
  - ESP32 development board
  - Gas sensor
  - Temperature sensor
  - Vibration sensor
  - SD card module

- **Software**:
  - PlatformIO
  - Arduino framework

## Installation Instructions

1. **Clone the Repository**:
    ```sh
    git clone https://github.com/s-a-usman/GasMonitorSystem.git
    cd GasMonitorSystem
    ```

2. **Open the Project in PlatformIO**:
    - Open PlatformIO IDE.
    - Open the cloned project folder.

3. **Configure Wi-Fi Credentials**:
    - Update the [ssid] and [password] variables in [main.cpp] with your Wi-Fi credentials.

4. **Build and Upload**:
    - Connect your ESP32 board to your computer.
    - Build and upload the project using PlatformIO.

## Usage

1. **Power On**:
    - Power on the ESP32 board. It will automatically connect to the configured Wi-Fi network.

2. **Access Web Interface**:
    - Open a web browser and navigate to the IP address displayed in the serial monitor.

3. **View Real-time Data**:
    - The web interface displays real-time gas levels, temperature, and vibration data.

4. **View Historical Data**:
    - Use the time controls on the web interface to switch between live data and historical data for different time ranges (1 hour, 24 hours, 1 month).


## API Endpoints

- **GET /api/data**: Returns the current sensor data in JSON format.
- **GET /api/historical-data?range={timeRange}**: Returns historical sensor data for the specified time range (`1h`, `24h`, `1m`).

## Safety Warnings

1. Ensure proper grounding for all electrical components.
2. Regularly test sensor accuracy and calibrate if necessary.
3. Follow local safety regulations for gas monitoring.

## Future Improvements

1. Add remote monitoring capabilities.
2. Implement additional sensors for enhanced monitoring.
3. Create a mobile app interface for easier access.


## Version History

### v1.0.0 (2025-02-10)
- Initial release
- All functionalities implemented
- Debug system added