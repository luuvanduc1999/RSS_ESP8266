#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "U8g2_for_Adafruit_GFX.h"
#include <WiFiUdp.h>

char ssid[] = "FPT-Unknown";   // your network SSID (name)
char pass[] = "hustbephot";   // your network password
int idx = 0;
WiFiClient  client;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "asia.pool.ntp.org", 25200);

#define TFT_CS    D2     // TFT CS  pin is connected to NodeMCU pin D2
#define TFT_RST   D3     // TFT RST pin is connected to NodeMCU pin D3
#define TFT_DC    D4     // TFT DC  pin is connected to NodeMCU pin D4
#define SWITCH    D6     // TFT DC  pin is connected to NodeMCU pin D4
// initialize ILI9341 TFT library with hardware SPI module
// SCK (CLK) ---> NodeMCU pin D5 (GPIO14)
// MOSI(DIN) ---> NodeMCU pin D7 (GPIO13)
Adafruit_ILI9341 display = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

char TINHOT[23][9] = {"18814688", "18814687", "18814686", "18803467", "18803466", "18803465", "18803464", "18803463", "18803461", "18803460", "18808019", "18808018", "18808017", "18808016", "18808015", "18808014", "18808013", "18808012", "18808011", "18808010", "18808009", "18808008", "18808007"};
char T_TINHOT[23][9] = {"18808067", "18808066", "18808065", "18808064", "18808063", "18808062", "18808061", "18808060", "18808058", "18808057", "18808056", "18808055", "18808053", "18808052", "18807815", "18807814", "18807813", "18807812", "18807811", "18807810", "18807809", "18807808", "18807807"};

U8G2_FOR_ADAFRUIT_GFX u8g2_for_adafruit_gfx;

unsigned long CmyTalkBackID = 38959;
const char * CmyTalkBackKey = "9DD7RLNE6Q5YZV90";

unsigned long TmyTalkBackID = 38964;
const char * TmyTalkBackKey = "JFIC48F3RU95H66U";


void setup() {
  

  display.begin();
  display.setRotation(1);
  display.fillScreen(ILI9341_BLACK);
  u8g2_for_adafruit_gfx.begin(display);
  u8g2_for_adafruit_gfx.setFontMode(0);                 // use u8g2 none transparent mode
  u8g2_for_adafruit_gfx.setFontDirection(0);            // left to right (this is default)


  Serial.begin(115200);          // Initialize serial


  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(String(ssid));
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass);
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConnected.");
  }
  pinMode(SWITCH, INPUT);
}

void setClock() {
  timeClient.update();
  Serial.print(timeClient.getHours());
  Serial.print(":");
  Serial.println(timeClient.getMinutes());
  String timex(timeClient.getHours());
  timex.concat(":");
  if (timeClient.getMinutes() < 10) {
    timex.concat("0");
    timex.concat(timeClient.getMinutes());
  }
  else
    timex.concat(timeClient.getMinutes());

  display.fillRect(260, 0, 330, 20, ILI9341_BLACK);
  u8g2_for_adafruit_gfx.setCursor(260, 20);
  u8g2_for_adafruit_gfx.setForegroundColor(ILI9341_WHITE);
  u8g2_for_adafruit_gfx.setFont(u8g2_font_helvR14_tn);
  u8g2_for_adafruit_gfx.print( timex);

}

// General function to POST to ThingSpeak
String httpGET(String uri, String postMessage) {

  bool connectSuccess = false;
  connectSuccess = client.connect("api.thingspeak.com", 80);

  if (!connectSuccess) {
    Serial.println("Khong ket noi");
  }

  postMessage += "&headers=false";

  String Headers =  String("GET ") + uri + String(" HTTP/1.1\r\n") +
                    String("Host: api.thingspeak.com\r\n") +
                    String("Content-Type: application/x-www-form-urlencoded\r\n") +
                    String("Connection: close\r\n") +
                    String("Content-Length: ") + String(postMessage.length()) +
                    String("\r\n\r\n");

  client.print(Headers);
  client.print(postMessage);

  long startWaitForResponseAt = millis();
  while (client.available() == 0 && millis() - startWaitForResponseAt < 5000) {
    delay(100);
  }

  if (client.available() == 0) {
    //return -304; // Didn't get server response in time
  }

  if (!client.find(const_cast<char *>("HTTP/1.1"))) {
    // return -303; // Couldn't parse response (didn't find HTTP/1.1)
  }

  int status = client.parseInt();
  if (status != 200) {
    //return status;
  }

  if (!client.find(const_cast<char *>("\n\r\n"))) {
    //return -303;
  }


  String tempString = String(client.readString());
  return tempString;

}

void jsonFT(String payload)
{
  const size_t bufferSize = JSON_OBJECT_SIZE(4)  + 370;
  DynamicJsonBuffer jsonBuffer(bufferSize);
  JsonObject& root = jsonBuffer.parseObject(payload);
  const char* rs = root["command_string"];

  const size_t bufferSizeArr = JSON_OBJECT_SIZE(2)  + 370;
  DynamicJsonBuffer jsonBufferArr(bufferSizeArr);
  JsonArray& array = jsonBufferArr.parseArray(rs);

  //const size_t bufferSize2 = JSON_OBJECT_SIZE(2)  + 370;
  //DynamicJsonBuffer jsonBuffer2(bufferSize2);
  //JsonObject& root2 = jsonBuffer2.parseObject(rs);
  //const char* title = root2["title"];

  u8g2_for_adafruit_gfx.setForegroundColor(ILI9341_GREEN);
  u8g2_for_adafruit_gfx.setFont(u8g2_font_open_iconic_arrow_2x_t  );
  u8g2_for_adafruit_gfx.drawGlyph( 2, 47, 78);

  u8g2_for_adafruit_gfx.setFont(u8g2_font_unifont_t_vietnamese2);

  const char* tmp = array[0];
  Serial.println(tmp);
  u8g2_for_adafruit_gfx.setCursor(15, 45);
  u8g2_for_adafruit_gfx.print(tmp); //max=38
  const char* tmp2 = array[1];
  u8g2_for_adafruit_gfx.setCursor(5, 65);
  u8g2_for_adafruit_gfx.print(tmp2); //max=38


}

void jsonFC(String payload)
{
  const size_t bufferSize = JSON_OBJECT_SIZE(4)  + 370;

  DynamicJsonBuffer jsonBuffer(bufferSize);
  JsonObject& root = jsonBuffer.parseObject(payload);
  const char* rs = root["command_string"];
  display.fillRect(2, 90, 330, 113, ILI9341_BLACK);
  const size_t bufferSizeArr = JSON_OBJECT_SIZE(5)  + 370;
  DynamicJsonBuffer jsonBufferArr(bufferSizeArr);
  JsonArray& array = jsonBufferArr.parseArray(rs);

  //const size_t bufferSize2 = JSON_OBJECT_SIZE(2)  + 370;
  //DynamicJsonBuffer jsonBuffer2(bufferSize2);
  //JsonObject& root2 = jsonBuffer2.parseObject(rs);
  //const char* title = root2["title"];

  u8g2_for_adafruit_gfx.setForegroundColor(ILI9341_WHITE);
  u8g2_for_adafruit_gfx.setFont(u8g2_font_unifont_t_vietnamese2);
  int line = 90;

  for (int i = 0; i < array.size(); i++)
  {
    const char* tmp = array[i];
    Serial.println(tmp);
    u8g2_for_adafruit_gfx.setCursor(5, line);
    u8g2_for_adafruit_gfx.print(tmp);
    line += 20;
  }

}

void getMostViewed( )
{
  setClock();
  if (idx == 0) {
    display.fillScreen(ILI9341_BLACK);
    u8g2_for_adafruit_gfx.setForegroundColor(ILI9341_RED);
    u8g2_for_adafruit_gfx.setFont(u8g2_font_open_iconic_www_2x_t);
    u8g2_for_adafruit_gfx.drawGlyph( 5, 234, 78);
    u8g2_for_adafruit_gfx.setCursor(280, 230);
    u8g2_for_adafruit_gfx.setFont(u8g2_font_helvR14_tf);
    u8g2_for_adafruit_gfx.setCursor(23, 232);
    u8g2_for_adafruit_gfx.print("VNEXPRESS.NET"); //max=38
    u8g2_for_adafruit_gfx.setCursor(2, 20);
    u8g2_for_adafruit_gfx.print("GROUP 26"); //max=38
  }
  idx = idx % 23;
  display.fillRect(280, 214, 330, 330, ILI9341_BLACK);
  display.fillRect(2, 30, 330, 173, ILI9341_BLACK);
  String index(String(idx + 1) + String("/23"));
  u8g2_for_adafruit_gfx.setCursor(280, 230);
  u8g2_for_adafruit_gfx.setFont(u8g2_font_unifont_t_vietnamese2);
  u8g2_for_adafruit_gfx.print(index);

  u8g2_for_adafruit_gfx.setForegroundColor(ILI9341_WHITE);
  u8g2_for_adafruit_gfx.setFont(u8g2_font_open_iconic_www_2x_t);
  u8g2_for_adafruit_gfx.drawGlyph( 150, 130, 76);

  String titleURI = String("/talkbacks/") + String(TmyTalkBackID) + String("/commands/") + String(T_TINHOT[idx]) + String(".json");
  String postMessage =  String("api_key=") + String(TmyTalkBackKey);
  String x = httpGET(titleURI, postMessage);
  jsonFT(x.substring(x.indexOf('{'), x.indexOf('}') + 1));


  String tbURI = String("/talkbacks/") + String(CmyTalkBackID) +  String("/commands/") + String(TINHOT[idx]) + String(".json");
  String tbpostMessage =  String("api_key=") + String(CmyTalkBackKey);
  String xx = httpGET(tbURI, tbpostMessage);
  jsonFC(xx.substring(xx.indexOf('{'), xx.indexOf('}') + 1));
  idx++;
}

void showClock( )
{
  if (idx == -1) {
    display.fillScreen(ILI9341_BLACK);
    u8g2_for_adafruit_gfx.setFontMode(0);                 // use u8g2 none transparent mode
    u8g2_for_adafruit_gfx.setFontDirection(0);            // left to right (this is default)
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
  }
  idx--;
}



void loop() {
  int rdxc = digitalRead(SWITCH);
  if ((rdxc == 1) && (idx >= 0)) idx = -1;
  if ((rdxc == 0) && (idx < 0)) idx = 0;
  Serial.println(idx);
  Serial.println(rdxc);
  if (idx >= 0) getMostViewed();
  else showClock();

  delay(2000);
}
