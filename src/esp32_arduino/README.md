# ESP32 Arduino code
This folder contains code to run on Arduino or ESP32 boards using Arduino IDE

## Requirements
- DHT humidity and temperature sensor
    - <Adafruit_Sensor.h>
    - <DHT.h> # DHT 11, 21, 22 sensors

- Cloud
    - <WiFi.h> # Connect to Internet
    - <WiFiClientSecure.h> # Set security for HTTP requests
    - <HTTPClient.h> # Handle HTTP requests
    - <Arduino_JSON.h> # Create JSON payload
    - "time.h" # Get timestamps

- [Optional] 
    - <Wire.h>, <Adafruit_GFX.h> and <Adafruit_SSD1306.h> used for showing the readings on an OLED display SSD1306.
