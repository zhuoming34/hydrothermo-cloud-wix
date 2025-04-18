#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <Arduino_JSON.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include "time.h"

#define SCREEN_WIDTH_PX 128
#define SCREEN_HEIGHT_PX 64

#define DHTPIN 15 // Digital pin connected to the DHT sensor
#define DHTTYPE    DHT11     // DHT 11
// #define DHTTYPE    DHT22     // DHT 22 (AM2302)
// #define DHTTYPE    DHT21     // DHT 21 (AM2301)

// TODO: Update API url, table name, device id, WiFi settings below
const char *deviceId = "YOUR-DEVICE-ID"; // <--- UPDATE THIS!!
const char *dynamoTable = "YOUR-DYNAMODB-TABLE-NAME"; // <--- UPDATE THIS!!
const char *postToDynamoURL = "YOUR-API-URL"; // <--- UPDATE THIS!!

// WiFi Settings
const char *ssid = "YOUR-WIFI-NAME"; // <--- UPDATE THIS!!
const char *password = "YOUR-WIFI-PASSWORD"; // <--- UPDATE THIS!!

// Time servers
const char *ntpServer1 = "pool.ntp.org";
const char *ntpServer2 = "time.nist.gov";
const long gmtOffset_sec = 0;
const int daylightOffset_sec = 0;
const char *time_zone = "EST5EDT,M3.2.0,M11.1.0";  // TimeZone rule for Eastern Time including daylight adjustment rules (optional)

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH_PX, SCREEN_HEIGHT_PX, &Wire, -1);

DHT dht(DHTPIN, DHTTYPE);

bool getTimestamp(tm* timeinfo, time_t* now) {
  if(!getLocalTime(timeinfo)){
    Serial.println("Failed to obtain time");
    return false;
  }
  time(now);
  
  Serial.println(timeinfo, "%A, %B %d %Y %H:%M:%S");
  Serial.println("Epoch Time: " + String(*now));
  return true;
}

void readData(float* temperature, float* humidity) {
  //read temperature and humidity
  *temperature = dht.readTemperature();
  *humidity = dht.readHumidity();
  if (isnan(*humidity) || isnan(*temperature)) {
    Serial.println("Failed to read from DHT sensor!");
  }
  Serial.println("Temperature: " + String(*temperature) + " C");
  Serial.println("Humidity: " + String(*humidity) + " %");
}

void dispData(float temperature, float humidity, char* localtime) {
  display.clearDisplay();
  
  // display time
  display.setCursor(0, 0);
  display.print("Time:");
  display.setCursor(0, 10);
  display.print(localtime);

  // display temperature
  display.setTextSize(1);
  display.setCursor(0,20);
  display.print("Temperature: ");
  display.setCursor(0,30);
  display.print(temperature);
  display.print(" ");
  display.cp437(true); // enale code page 437
  display.write(167); // degree symbol
  display.print("C");
  
  // display humidity
  display.setCursor(0, 40);
  display.print("Humidity: ");
  display.setCursor(0, 50);
  display.print(humidity);
  display.print(" %"); 
  
  display.display();
}

void setup() {
  Serial.begin(115200);

  // Set up WiFi
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("CONNECTED");

  // Set up time server
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2);
  configTzTime(time_zone, ntpServer1, ntpServer2);

  // Set up DHT
  dht.begin();

  // Set up display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();
  display.setTextColor(WHITE);
}

void loop() {
  float temperature, humidity;
  struct tm timeinfo;
  time_t now;
  char timebuffer[20];
  JSONVar httpPayload;
  JSONVar envData;
  bool isTimeObtained = false;

  WiFiClientSecure wifiClient;
  wifiClient.setInsecure();  // For quick testing, disables SSL certificate checks
  HTTPClient httpClient;

  while (!isTimeObtained) {
    isTimeObtained = getTimestamp(&timeinfo, &now);
  }
  strftime(timebuffer, sizeof(timebuffer), "%m/%d/%Y %H:%M:%S", &timeinfo);
  Serial.println(timebuffer);

  readData(&temperature, &humidity);
  dispData(temperature, humidity, timebuffer);

  httpPayload["table_name"] = dynamoTable;
  httpPayload["device_id"] = deviceId;
  httpPayload["timestamp"] = String(now);
  envData["temperature"] = roundf(temperature * 100) / 100.0;
  envData["humidity"] = roundf(humidity * 100) / 100.0;
  httpPayload["env_data"] = envData;
  String serialized_payload = JSON.stringify(httpPayload);

  httpClient.begin(wifiClient, postToDynamoURL);
  httpClient.addHeader("Content-Type", "application/json");
  int httpResponseCode = httpClient.POST(serialized_payload);
  Serial.println("HTTP Response code: " + String(httpResponseCode));
  httpClient.end();

  delay(900000);
}
