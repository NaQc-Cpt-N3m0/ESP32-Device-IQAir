#include <Arduino.h>

// Khai báo thư viện Wifi
#include "WiFi.h"            // Thư viện WiFi cho ESP32
#include <HTTPClient.h>      // Thư viện gửi HTTP request (GET/POST)
#include <ArduinoJson.h>     // Thư viện parse JSON

// Khai báo thư viện Oled LCD
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Khai báo thư viện cho quản lý Wifi
#include <WiFiManager.h>


// Thư viện lấy thời gian: 
#include "time.h"

// múi giờ Việt Nam
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 7 * 3600;
const int   daylightOffset_sec = 0;

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



#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES     10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
static const unsigned char PROGMEM logo_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };

void testdrawline() {
  int16_t i;

  display.clearDisplay(); // Clear display buffer

  for(i=0; i<display.width(); i+=4) {
    display.drawLine(0, 0, i, display.height()-1, WHITE);
    display.display(); // Update screen with each newly-drawn line
    delay(1);
  }
  for(i=0; i<display.height(); i+=4) {
    display.drawLine(0, 0, display.width()-1, i, WHITE);
    display.display();
    delay(1);
  }
  delay(250);

  display.clearDisplay();

  for(i=0; i<display.width(); i+=4) {
    display.drawLine(0, display.height()-1, i, 0, WHITE);
    display.display();
    delay(1);
  }
  for(i=display.height()-1; i>=0; i-=4) {
    display.drawLine(0, display.height()-1, display.width()-1, i, WHITE);
    display.display();
    delay(1);
  }
  delay(250);

  display.clearDisplay();

  for(i=display.width()-1; i>=0; i-=4) {
    display.drawLine(display.width()-1, display.height()-1, i, 0, WHITE);
    display.display();
    delay(1);
  }
  for(i=display.height()-1; i>=0; i-=4) {
    display.drawLine(display.width()-1, display.height()-1, 0, i, WHITE);
    display.display();
    delay(1);
  }
  delay(250);

  display.clearDisplay();

  for(i=0; i<display.height(); i+=4) {
    display.drawLine(display.width()-1, 0, 0, i, WHITE);
    display.display();
    delay(1);
  }
  for(i=0; i<display.width(); i+=4) {
    display.drawLine(display.width()-1, 0, i, display.height()-1, WHITE);
    display.display();
    delay(1);
  }

  delay(2000); // Pause for 2 seconds
}

void testdrawrect(void) {
  display.clearDisplay();

  for(int16_t i=0; i<display.height()/2; i+=2) {
    display.drawRect(i, i, display.width()-2*i, display.height()-2*i, WHITE);
    display.display(); // Update screen with each newly-drawn rectangle
    delay(1);
  }

  delay(2000);
}

void testfillrect(void) {
  display.clearDisplay();

  for(int16_t i=0; i<display.height()/2; i+=3) {
    // The INVERSE color is used so rectangles alternate white/black
    display.fillRect(i, i, display.width()-i*2, display.height()-i*2, INVERSE);
    display.display(); // Update screen with each newly-drawn rectangle
    delay(1);
  }

  delay(2000);
}

void testdrawcircle(void) {
  display.clearDisplay();

  for(int16_t i=0; i<max(display.width(),display.height())/2; i+=2) {
    display.drawCircle(display.width()/2, display.height()/2, i, WHITE);
    display.display();
    delay(1);
  }

  delay(2000);
}

void testfillcircle(void) {
  display.clearDisplay();

  for(int16_t i=max(display.width(),display.height())/2; i>0; i-=3) {
    // The INVERSE color is used so circles alternate white/black
    display.fillCircle(display.width() / 2, display.height() / 2, i, INVERSE);
    display.display(); // Update screen with each newly-drawn circle
    delay(1);
  }

  delay(2000);
}

void testdrawroundrect(void) {
  display.clearDisplay();

  for(int16_t i=0; i<display.height()/2-2; i+=2) {
    display.drawRoundRect(i, i, display.width()-2*i, display.height()-2*i,
      display.height()/4, WHITE);
    display.display();
    delay(1);
  }

  delay(2000);
}

void testfillroundrect(void) {
  display.clearDisplay();

  for(int16_t i=0; i<display.height()/2-2; i+=2) {
    // The INVERSE color is used so round-rects alternate white/black
    display.fillRoundRect(i, i, display.width()-2*i, display.height()-2*i,
      display.height()/4, INVERSE);
    display.display();
    delay(1);
  }

  delay(2000);
}

void testdrawtriangle(void) {
  display.clearDisplay();

  for(int16_t i=0; i<max(display.width(),display.height())/2; i+=5) {
    display.drawTriangle(
      display.width()/2  , display.height()/2-i,
      display.width()/2-i, display.height()/2+i,
      display.width()/2+i, display.height()/2+i, WHITE);
    display.display();
    delay(1);
  }

  delay(2000);
}

void testfilltriangle(void) {
  display.clearDisplay();

  for(int16_t i=max(display.width(),display.height())/2; i>0; i-=5) {
    // The INVERSE color is used so triangles alternate white/black
    display.fillTriangle(
      display.width()/2  , display.height()/2-i,
      display.width()/2-i, display.height()/2+i,
      display.width()/2+i, display.height()/2+i, INVERSE);
    display.display();
    delay(1);
  }

  delay(2000);
}

void testdrawchar(void) {
  display.clearDisplay();

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  // Not all the characters will fit on the display. This is normal.
  // Library will draw what it can and the rest will be clipped.
  for(int16_t i=0; i<256; i++) {
    if(i == '\n') display.write(' ');
    else          display.write(i);
  }

  display.display();
  delay(2000);
}

void testdrawstyles(void) {
  display.clearDisplay();

  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println(F("Hello, world!"));

  display.setTextColor(BLACK, WHITE); // Draw 'inverse' text
  display.println(3.141592);

  display.setTextSize(2);             // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.print(F("0x")); display.println(0xDEADBEEF, HEX);

  display.display();
  delay(2000);
}

void testscrolltext(void) {
  display.clearDisplay();

  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.setCursor(10, 0);
  display.println(F("scroll"));
  display.display();      // Show initial text
  delay(100);

  // Scroll in various directions, pausing in-between:
  display.startscrollright(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrollleft(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrolldiagright(0x00, 0x07);
  delay(2000);
  display.startscrolldiagleft(0x00, 0x07);
  delay(2000);
  display.stopscroll();
  delay(1000);
}

void testdrawbitmap(void) {
  display.clearDisplay();

  display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2,
    (display.height() - LOGO_HEIGHT) / 2,
    logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display();
  delay(1000);
}

#define XPOS   0 // Indexes into the 'icons' array in function below
#define YPOS   1
#define DELTAY 2

void testanimate(const uint8_t *bitmap, uint8_t w, uint8_t h) {
  int8_t f, icons[NUMFLAKES][3];

  // Initialize 'snowflake' positions
  for(f=0; f< NUMFLAKES; f++) {
    icons[f][XPOS]   = random(1 - LOGO_WIDTH, display.width());
    icons[f][YPOS]   = -LOGO_HEIGHT;
    icons[f][DELTAY] = random(1, 6);
    Serial.print(F("x: "));
    Serial.print(icons[f][XPOS], DEC);
    Serial.print(F(" y: "));
    Serial.print(icons[f][YPOS], DEC);
    Serial.print(F(" dy: "));
    Serial.println(icons[f][DELTAY], DEC);
  }

  for(;;) { // Loop forever...
    display.clearDisplay(); // Clear the display buffer

    // Draw each snowflake:
    for(f=0; f< NUMFLAKES; f++) {
      display.drawBitmap(icons[f][XPOS], icons[f][YPOS], bitmap, w, h, WHITE);
    }

    display.display(); // Show the display buffer on the screen
    delay(200);        // Pause for 1/10 second

    // Then update coordinates of each flake...
    for(f=0; f< NUMFLAKES; f++) {
      icons[f][YPOS] += icons[f][DELTAY];
      // If snowflake is off the bottom of the screen...
      if (icons[f][YPOS] >= display.height()) {
        // Reinitialize to a random position, just off the top
        icons[f][XPOS]   = random(1 - LOGO_WIDTH, display.width());
        icons[f][YPOS]   = -LOGO_HEIGHT;
        icons[f][DELTAY] = random(1, 6);
      }
    }
  }
}




// Xử lý cấu hình Wifi
void wifi_conf()
{
   WiFiManager wm;
    // reset settings - wipe stored credentials for testing
    // these are stored by the esp library
    //wm.resetSettings();

    // Automatically connect using saved credentials,
    // if connection fails, it starts an access point with the specified name ( "AutoConnectAP"),
    // if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.autoConnect())
    // then goes into a blocking loop awaiting configuration and will return success result

    bool res;
    // res = wm.autoConnect(); // auto generated AP name from chipid
    // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
    res = wm.autoConnect("AutoConnectAP","password"); // password protected ap

    if(!res) {
        Serial.println("Failed to connect");
        // ESP.restart();
    } 
    else {
        //if you get here you have connected to the WiFi    
        Serial.println("connected...yeey :)");
    }

}

// Hiện thị Oled
void display_oled_text(String text, uint8_t line)
{
    uint8_t textSize = 1;
    uint8_t lineHeight = 8 * textSize;   // 24px cho size 3
    uint8_t yPos = line * lineHeight;    

    display.setTextSize(textSize);
    display.setTextColor(WHITE);
    display.setCursor(0, yPos);

    display.println(text);
    display.display();
}



void setup() {

  // Cấu hình Wifi

  Serial.begin(115200);
  delay(1000);

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  // Gọi API
  //getIQAirData();

  // Khai báo chân I2C TÙY Ý
  Wire.begin(22, 23);   // SDA = GPIO21, SCL = GPIO22


  // Xử lý phần màn hình Oled

  /**************************************************
   * KHỞI TẠO MÀN HÌNH OLED SSD1306
   **************************************************/

  // SSD1306_SWITCHCAPVCC:
  // - OLED tự tạo điện áp hoạt động bên trong
  // - Nguồn cấp chỉ cần 3.3V
  //
  // 0x3C:
  // - Địa chỉ I2C của màn hình OLED
  // - Phổ biến nhất là 0x3C (một số màn là 0x3D)
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Nếu khởi tạo OLED thất bại thì đứng yên
  }


  /**************************************************
   * HIỂN THỊ MÀN HÌNH KHỞI ĐỘNG (SPLASH SCREEN)
   **************************************************/

  // Hiển thị buffer mặc định của thư viện
  // (logo Adafruit)
  display.display();
  delay(2000); // Dừng 2 giây

  /**************************************************
   * XÓA MÀN HÌNH
   **************************************************/

  // Xóa toàn bộ bộ nhớ đệm (buffer)
  // Lúc này màn hình vẫn CHƯA đổi
  display.clearDisplay();
  display.display(); 


  /**************************************************
   * Cấu hình Wifi khi khởi động
   **************************************************/ 
    WiFiManager wm;
    // reset settings - wipe stored credentials for testing
    // these are stored by the esp library
    //wm.resetSettings();

    // Automatically connect using saved credentials,
    // if connection fails, it starts an access point with the specified name ( "AutoConnectAP"),
    // if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.autoConnect())
    // then goes into a blocking loop awaiting configuration and will return success result
    display_oled_text("Config Wifi", 0);   // dòng 1
    display_oled_text("name: AutoConnectAP ", 1);   // dòng 1
    display_oled_text("pw: password ", 2);   // dòng 1
    bool res;
    // res = wm.autoConnect(); // auto generated AP name from chipid
    // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
    res = wm.autoConnect("AutoConnectAP","password"); // password protected ap

    if(!res) {
        Serial.println("Failed to connect");
        // ESP.restart();
        display_oled_text("==> Failed to connect", 4);   // dòng 1
    } 
    else {
        //if you get here you have connected to the WiFi    
        Serial.println("connected...yeey :)");
        display_oled_text("==> connected wifi", 4);  
    }
}

void loop() {
  // put your main code here, to run repeatedly:
  // display.clearDisplay();
  // display.display(); 

  while (1)
  {
    // Lấy dữ liệu API từ IQ Air
    display.clearDisplay();
    display.display(); 

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

        display.clearDisplay();

        display.setTextSize(1);
        display.setTextColor(WHITE);

        display.setCursor(0, 0);
        display.print("Quan: ");
        display.println(city);

        display.setCursor(0, 12);
        display.print("TP: ");
        display.println(state);

        display.setCursor(0, 24);
        display.print("Quoc gia: ");
        display.println(country);

        display.setCursor(0, 36);
        display.print("AQI: ");
        display.println(aqi);

        struct tm timeinfo;
        if (getLocalTime(&timeinfo)) {
            Serial.printf("Date/Time: %02d/%02d/%04d %02d:%02d:%02d\n",
            timeinfo.tm_mday,
            timeinfo.tm_mon + 1,
            timeinfo.tm_year + 1900,
            timeinfo.tm_hour,
            timeinfo.tm_min,
            timeinfo.tm_sec);


            char buffer[16];
            sprintf(buffer, "%02d:%02d:%02d",
                timeinfo.tm_hour,
                timeinfo.tm_min,
                timeinfo.tm_sec
            );

            display_oled_text(buffer, 6); // dòng 5
        }

        display.display();


      } else {
        Serial.println("Loi parse JSON!");
      }
    }
  } else {
    Serial.printf("HTTP GET failed: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
    
    delay(60000);
  }
  

}

