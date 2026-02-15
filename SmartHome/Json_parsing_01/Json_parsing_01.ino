#include <ArduinoJson.h>
#define LED 5

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
}

void loop() {
  if(Serial.available()){
    //line feed (\n)까지 받음(보내는 쪽에서 \n 안붙이면 default setTimeout() 1초)
    
    String data = Serial.readStringUntil(0x0a);
    Serial.println(data);
    StaticJsonDocument<48> doc;
    deserializeJson(doc, data);
    String led_num = doc["num"];
    String led_set = doc["set"]; 


    Serial.println(led_num + "," + led_set);
    if(led_num == "1"){
      if(led_set == "on")
      {
          digitalWrite(LED, HIGH);
      }else if(led_set == "off")
      {
        digitalWrite(LED, LOW);  
      }
    }

  }
}
