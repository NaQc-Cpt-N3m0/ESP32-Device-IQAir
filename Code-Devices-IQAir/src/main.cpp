#include <Arduino.h>

// Khai báo thư viện Wifi
#include "WiFi.h"            // Thư viện WiFi cho ESP32
#include <HTTPClient.h>      // Thư viện gửi HTTP request (GET/POST)
#include <ArduinoJson.h>     // Thư viện parse JSON

// Khai báo thư viện Oled LCD
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


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

  /**************************************************
   * VẼ 1 ĐIỂM ẢNH (PIXEL)
   **************************************************/

  // Vẽ 1 pixel màu trắng tại tọa độ (x=10, y=10)
  display.drawPixel(10, 10, WHITE);

  // Show the display buffer on the screen. You MUST call display() after
  // Đẩy buffer ra màn hình để hiển thị pixel
  display.display();
  delay(2000);

  // display.display() is NOT necessary after every single drawing command,
  // unless that's what you want...rather, you can batch up a bunch of
  // drawing operations and then update the screen all at once by calling
  // display.display(). These examples demonstrate both approaches...

  Serial.println("Vẽ nhiều đường thẳng");
  testdrawline();      // Vẽ nhiều đường thẳng

  Serial.println("Vẽ nhiều đường thẳng");
  testdrawrect();      // Vẽ nhiều đường thẳng

  Serial.println("Vẽ hình chữ nhật (tô đầy)");
  testfillrect();      // Vẽ hình chữ nhật (tô đầy)

  Serial.println("Vẽ hình tròn (viền)");
  testdrawcircle();    /// Vẽ hình tròn (viền)

  Serial.println("Vẽ hình tròn (tô đầy)");
  testfillcircle();    // Vẽ hình tròn (tô đầy)

  Serial.println("Vẽ hình chữ nhật bo góc (viền)");
  testdrawroundrect(); // Vẽ hình chữ nhật bo góc (viền)

  Serial.println("Vẽ hình chữ nhật bo góc (tô đầy)");
  testfillroundrect(); // Vẽ hình chữ nhật bo góc (tô đầy)

  Serial.println("Vẽ tam giác (viền)");
  testdrawtriangle();  // Vẽ tam giác (viền)

  Serial.println("Vẽ tam giác (tô đầy)");
  testfilltriangle();  // Vẽ tam giác (tô đầy)

  Serial.println("Vẽ ký tự đơn (font mặc định)");
  testdrawchar();      /// Vẽ ký tự đơn (font mặc định)

  Serial.println("// Vẽ chữ với nhiều kiểu (size, màu)");
  testdrawstyles();    // Vẽ chữ với nhiều kiểu (size, màu)

  Serial.println("Demo chữ chạy (scroll)");
  testscrolltext();    // Demo chữ chạy (scroll)
  
  Serial.println("Vẽ ảnh bitmap nhỏ");
  testdrawbitmap();    // Vẽ ảnh bitmap nhỏ

  // Invert and restore display, pausing in-between
  display.invertDisplay(true);
  delay(1000);
  display.invertDisplay(false);
  delay(1000);

  /**************************************************
   * ANIMATION BITMAP
   **************************************************/

  // Hiển thị ảnh bitmap động (animation)
  // logo_bmp      : mảng dữ liệu ảnh
  // LOGO_WIDTH    : chiều rộng ảnh
  // LOGO_HEIGHT   : chiều cao ảnh
  testanimate(logo_bmp, LOGO_WIDTH, LOGO_HEIGHT); // Animate bitmaps
  
}

void loop() {
  // put your main code here, to run repeatedly:
}

