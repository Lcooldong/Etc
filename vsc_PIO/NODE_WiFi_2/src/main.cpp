#include <Arduino.h>
#include "WiFiEsp.h"
#include <SoftwareSerial.h> 
#define rxPin 3 
#define txPin 2 
SoftwareSerial esp01(txPin, rxPin); // SoftwareSerial NAME(TX, RX);

const char ssid[] = "SK_WiFiGIGA40F7";    // your network SSID (name)
const char pass[] = "1712037218";         // your network password
int status = WL_IDLE_STATUS;        // the Wifi radio's status

#define ledPin 13
#define digiPin 7

void pin_control();

WiFiEspServer server(80);

uint8_t pin_val = 0; // 디지털 버튼 제어용 변수
String text = "";



void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(digiPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  digitalWrite(digiPin, LOW);
  Serial.begin(9600);   // initialize serial for debugging
  esp01.begin(9600);   //와이파이 시리얼
  WiFi.init(&esp01);   // initialize ESP module
  while ( status != WL_CONNECTED) {   // 약 10초동안 wifi 연결 시도
    Serial.print(F("Attempting to connect to WPA SSID: "));
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);    // Connect to WPA/WPA2 network
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);
  }
  server.begin();
}

const char HTTP_HEAD[] PROGMEM     = "<!DOCTYPE html><html lang=\"en\"><head>"
                                     "<meta name=\"viewport\"content=\"width=device-width,initial-scale=1,user-scalable=no\"/>"
                                     "<link rel=\"icon\" href=\"data:,\">";
const char HTTP_STYLE[] PROGMEM    = "<style>"
                                     "body{text-align:center;font-family:verdana;}"  
                                     "button{border:0;border-radius:0.3rem;background-color:#1fa3ec;color:#fff;line-height:2.4rem;font-size:1.2rem;width:100%} "
                                     "</style>";
const char HTTP_HEAD_END[] PROGMEM = "</head><body><div style=\"text-align:center;display:inline-block;min-width:260px;\">"
                                     "<h3>ESP01 Digital Pin Control</h3>";
const char BUTTON_TYPE[] PROGMEM   = "<p><button style=\"width:40%;background-color:#12cb3d;\">ON</button>"
                                     "<button style=\"margin-left:10%;width:40%;background-color:#1fa3ec;\">OFF</button></a></p>";
const char BUTTON_A_ON[] PROGMEM   = "<p>Button A</p><a href=\"/A/0\"><button style=\"background-color:#12cb3d;\">ON</button></a></p>";
const char BUTTON_A_OFF[] PROGMEM  = "<p>Button A</p><a href=\"/A/1\"><button style=\"background-color:#1fa3ec;\">OFF</button></a></p>";
const char BUTTON_B_ON[] PROGMEM   = "<p>Button B</p><a href=\"/B/0\"><button style=\"background-color:#12cb3d;\">ON</button></a></p>";
const char BUTTON_B_OFF[] PROGMEM  = "<p>Button B</p><a href=\"/B/1\"><button style=\"background-color:#1fa3ec;\">OFF</button></a></p>";
const char HTTP_END[] PROGMEM      = "</div></body></html>\r\n";

bool button_a = LOW; // off
bool button_b = LOW; // off

void loop() {
  WiFiEspClient client = server.available();  // listen for incoming clients
  if (client) {                               // if you get a client,
    while (client.connected()) {              // loop while the client's connected
      if (client.available()) {               // if there's bytes to read from the client,
        String income_wifi = client.readStringUntil('\n');
        bool browser = false;
        if (income_wifi.indexOf("%%F0") != -1) {
            String wifi_temp = income_wifi.substring(income_wifi.indexOf("%%F0")+4, income_wifi.indexOf("%%F1"));
            pin_val = wifi_temp.toInt();
            pin_control();
        } else if (income_wifi.indexOf(F("A/1")) != -1) {
          Serial.println(F("button_A on"));
          button_a = HIGH; browser = true;
          digitalWrite(ledPin, button_a);
        } else if (income_wifi.indexOf(F("A/0")) != -1) {
          Serial.println(F("button_A off"));
          button_a = LOW; browser = true;
          digitalWrite(ledPin, button_a);
        } else if (income_wifi.indexOf(F("B/1")) != -1) {
          Serial.println(F("button_B on"));
          button_b = HIGH; browser = true;
          digitalWrite(digiPin, button_b);
        } else if (income_wifi.indexOf(F("B/0")) != -1) {
          Serial.println(F("button_B off"));
          button_b = LOW; browser = true;
          digitalWrite(digiPin, button_b);
        } else {
          String wifi_temp = income_wifi.substring(income_wifi.indexOf("GET /")+5, income_wifi.indexOf("HTTP/1.1"));
          if (wifi_temp != " ") {
            if (wifi_temp.indexOf("%20") != -1) {
              String space = "%20";
              String space_convert = " ";
              wifi_temp.replace(space, space_convert);
            }
            text = wifi_temp;
            Serial.println(text);
          } else {
            browser = true;
          }
        }
        client.flush();
        if (browser == true) {
          client.println(F("HTTP/1.1 200 OK"));  // HTTP 프로토콜 헤더
          client.println(F("Content-type:text/html"));
          client.println(F("Connection: close"));
          client.println();
          String page;
          page  = (const __FlashStringHelper *)HTTP_HEAD;
          page += (const __FlashStringHelper *)HTTP_STYLE;
          page += (const __FlashStringHelper *)HTTP_HEAD_END;
          page += (const __FlashStringHelper *)BUTTON_TYPE;
          if (button_a == HIGH) {
            page += (const __FlashStringHelper *)BUTTON_A_ON;
          } else {
            page += (const __FlashStringHelper *)BUTTON_A_OFF;
          }
          if (button_b == HIGH) {
            page += (const __FlashStringHelper *)BUTTON_B_ON;
          } else {
            page += (const __FlashStringHelper *)BUTTON_B_OFF;
          }
          page += (const __FlashStringHelper *)HTTP_END;
          client.print(page);
          client.println();
          delay(1);
        }
        break;
      }
    }
    client.stop();
    Serial.println(F("Client disconnected"));
  }
}

void pin_control() {
  if (pin_val != 0) {
    switch (pin_val) {
      case 11: digitalWrite(ledPin, HIGH); // button 1 : on
               Serial.println("App Button_A ON");
                  break;
      case 10: digitalWrite(ledPin, LOW); // button 1 : off
               Serial.println("App Button_A OFF");
                  break;
      case 21: Serial.println("button 2 : on");
               Serial.println("App Button_B ON");
                  break;
      case 20: Serial.println("button 2 : off");
               Serial.println("App Button_B OFF");
                  break;
    }
  pin_val = 0;
  }
}