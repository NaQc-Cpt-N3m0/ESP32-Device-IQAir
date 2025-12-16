#include <Arduino.h>
// Khai báo thư viện Wifi
#include "WiFi.h"            // Thư viện WiFi cho ESP32
#include <HTTPClient.h>      // Thư viện gửi HTTP request (GET/POST)
#include <ArduinoJson.h>     // Thư viện parse JSON

// Khai báo thư viện Oled LCD
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(115200);
  Wire.begin(22, 23);   // SDA = GPIO21, SCL = GPIO22
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();

  display.setTextSize(3);             // Kích thước chữ
  display.setTextColor(WHITE); 
  display.setCursor(0, 10);           // Bắt đầu từ tạo độ nào
  // Display static text
  display.println("Hello, world!"); 
  display.display();                  // Hiện thị những gì đã setup


}

void loop() {
  // put your main code here, to run repeatedly:
}

