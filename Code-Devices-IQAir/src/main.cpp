#include <Arduino.h>
#include "WiFi.h"            // Thư viện WiFi cho ESP32
#include <HTTPClient.h>      // Thư viện gửi HTTP request (GET/POST)
#include <ArduinoJson.h>     // Thư viện parse JSON


#define ledC 2

// Cấu hình Wifi
const char* ssid = "Dinh Le Na";
const char* password = "hetmangroi";


// API - Và các cấu hình trên IQ Air
String apiKey = "0eb90420-c2c8-489d-ada1-227fc9554076";

void getIQAirData() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi chua ket noi!");
    return;
  }

  HTTPClient http;

  String url = "http://api.airvisual.com/v2/nearest_city?key=" + apiKey;
  Serial.println("GET: " + url);

  http.begin(url);
  int httpCode = http.GET();

  if (httpCode > 0) {
    Serial.printf("HTTP Code: %d\n", httpCode);

    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println(payload);

      // Parse JSON
      StaticJsonDocument<2048> doc;
      DeserializationError error = deserializeJson(doc, payload);

      if (!error) {
        const char* city    = doc["data"]["city"];
        const char* state    = doc["data"]["state"];
        const char* country = doc["data"]["country"];
        int aqi             = doc["data"]["current"]["pollution"]["aqius"];

        Serial.println("------ IQAir ------");
        Serial.print("Quận: "); Serial.println(city);
        Serial.print("Thành phố: "); Serial.println(state);
        Serial.print("Quốc gia: "); Serial.println(country);
        Serial.print("AQI (US): "); Serial.println(aqi);
      } else {
        Serial.println("Loi parse JSON!");
      }
    }
  } else {
    Serial.printf("HTTP GET failed: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
}

void blinkled (int de_pin, int ms)
{
  digitalWrite(ledC, LOW);
  delay(ms);
  digitalWrite(ledC, HIGH);
  delay(ms);
}

void setup() {

  // Cấu hình Wifi

  Serial.begin(115200);
  delay(1000);

  Serial.println("Dang ket noi WiFi...");
  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED)   // Kiểm tra kết nối Wifi
  {
    delay(500);
    Serial.print(".");

  }
  
  Serial.println();
  Serial.println("Da ket noi WiFi!");
  Serial.print("IP ESP32: ");
  Serial.println(WiFi.localIP());
  

   // Gọi API
  getIQAirData();
  
}

void loop() {
  // put your main code here, to run repeatedly:
}

