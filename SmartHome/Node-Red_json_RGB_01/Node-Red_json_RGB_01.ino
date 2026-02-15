#include <ArduinoJson.h>
#define LED_R 9
#define LED_G 11
#define LED_B 10

void setup() {
  Serial.begin(115200);
//  pinMode(LED_R, OUTPUT);
//  pinMode(LED_G, OUTPUT);
//  pinMode(LED_B, OUTPUT);
  ledcAttachPin(LED_R, 0);  //pin, channel
  ledcAttachPin(LED_G, 1);
  ledcAttachPin(LED_B, 2);

  ledcSetup(0, 5000, 8);  // channel, Hz(보통 5000), bit(해상도)
  ledcSetup(1, 5000, 8);
  ledcSetup(2, 5000, 8);
  
}

void loop() {
  if(Serial.available()){
    String data = Serial.readStringUntil(0x0a);

    //{"R":"R_data", "G":"G_data", "B":"B_data"}
    StaticJsonDocument<64> doc; 
    deserializeJson(doc, data);
    
    String R = doc["R"]; // "R_data"
    String G = doc["G"]; // "G_data"
    String B = doc["B"]; // "B_data"

//    analogWrite(LED_R, R.toInt());
//    analogWrite(LED_G, G.toInt());
//    analogWrite(LED_B, B.toInt());
    ledcWrite(0, R.toInt());
    ledcWrite(1, G.toInt());
    ledcWrite(2, B.toInt());
      
  }

}

void setColorCh(int redCh, int greenCh, int blueCh){
  ledcAttachPin(LED_R, redCh);  //pin, channel
  ledcAttachPin(LED_G, greenCh);
  ledcAttachPin(LED_B, blueCh);

  ledcSetup(redCh, 5000, 8);  // channel, Hz(보통 5000), bit(해상도)
  ledcSetup(greenCh, 5000, 8);
  ledcSetup(blueCh, 5000, 8);
}

void setColor(String red, String green, String blue){
  ledcWrite(0, red.toInt());
  ledcWrite(1, green.toInt());
  ledcWrite(2, blue.toInt());
}
