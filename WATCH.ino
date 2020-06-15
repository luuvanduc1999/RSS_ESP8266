#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "U8g2_for_Adafruit_GFX.h"


#define TFT_CS    D2     // TFT CS  pin is connected to NodeMCU pin D2
#define TFT_RST   D3     // TFT RST pin is connected to NodeMCU pin D3
#define TFT_DC    D4     // TFT DC  pin is connected to NodeMCU pin D4
#define SWITCH    D6     // TFT DC  pin is connected to NodeMCU pin D4
// initialize ILI9341 TFT library with hardware SPI module
// SCK (CLK) ---> NodeMCU pin D5 (GPIO14)
// MOSI(DIN) ---> NodeMCU pin D7 (GPIO13)
Adafruit_ILI9341 display = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);


U8G2_FOR_ADAFRUIT_GFX u8g2_for_adafruit_gfx;

void setup() {
  display.begin();
  Serial.begin(115200);
  display.setRotation(1);
  display.fillScreen(ILI9341_BLACK);
  u8g2_for_adafruit_gfx.begin(display);                 // connect u8g2 procedures to Adafruit GFX
  pinMode(SWITCH, INPUT);
}

unsigned long x = 0;

void loop() {
  int rd = digitalRead(SWITCH);
`  u8g2_for_adafruit_gfx.setFontMode(0);                 // use u8g2 none transparent mode
  u8g2_for_adafruit_gfx.setFontDirection(0);            // left to right (this is default)
  u8g2_for_adafruit_gfx.setForegroundColor(ILI9341_CYAN);
  display.drawRect(0, 0, 320, 45, ILI9341_WHITE);
  //  //  u8g2_for_adafruit_gfx.drawGlyph( 2,47,78);
  //  u8g2_for_adafruit_gfx.setFont(u8g2_font_open_iconic_www_2x_t);
  //  u8g2_for_adafruit_gfx.drawGlyph( 2,234,77);
  u8g2_for_adafruit_gfx.setCursor(118, 107);
  u8g2_for_adafruit_gfx.setFont(u8g2_font_helvB24_tf   );
  u8g2_for_adafruit_gfx.print( "01:23");
  u8g2_for_adafruit_gfx.setForegroundColor(ILI9341_CYAN);
  u8g2_for_adafruit_gfx.setFont(u8g2_font_helvR14_tf   );
  u8g2_for_adafruit_gfx.setCursor(70, 135);
  u8g2_for_adafruit_gfx.print( "Monday, 08/06/2020");

  u8g2_for_adafruit_gfx.setForegroundColor(ILI9341_RED);
  u8g2_for_adafruit_gfx.setFont(u8g2_font_helvR18_tf   );
  u8g2_for_adafruit_gfx.setCursor(50, 31);
  u8g2_for_adafruit_gfx.print( "GROUP 26 #KTVXL");

  u8g2_for_adafruit_gfx.setForegroundColor(ILI9341_ORANGE);
  u8g2_for_adafruit_gfx.setFont(u8g2_font_open_iconic_weather_6x_t);
  u8g2_for_adafruit_gfx.drawGlyph( 30, 230, 69);
  u8g2_for_adafruit_gfx.setForegroundColor(ILI9341_WHITE);
  u8g2_for_adafruit_gfx.setFont(u8g2_font_helvR14_tf  );
  u8g2_for_adafruit_gfx.setCursor(107, 200);
  u8g2_for_adafruit_gfx.print( "Temperature");
  u8g2_for_adafruit_gfx.setCursor(137, 220);
  u8g2_for_adafruit_gfx.print( "32°C");

  u8g2_for_adafruit_gfx.setCursor(230, 200);
  u8g2_for_adafruit_gfx.print( "Humidity");
  u8g2_for_adafruit_gfx.setCursor(245, 220);
  u8g2_for_adafruit_gfx.print( "65%");


  //display.drawRect(0,175,320,65,ILI9341_WHITE);
  //display.drawRect(0,175,213,65,ILI9341_WHITE);
  //display.drawRect(0,175,107,65,ILI9341_WHITE);


  x++;
  delay(1000);
}
