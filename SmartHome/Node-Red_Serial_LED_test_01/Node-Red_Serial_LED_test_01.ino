#define LED 23

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);

}

void loop() {
  if(Serial.available())
  {
    String data = Serial.readStringUntil(0x0a);

    if(data == "0")
    {
      digitalWrite(LED, LOW); 
    }else if(data == "1")
    {
      digitalWrite(LED, HIGH);
    }
  }
}
