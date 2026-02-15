#define button1 23
#define button2 22

void setup() {
  Serial.begin(115200);
  // PULLUP- 기본 HIGH
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);

}

void loop() {
  bool btn1 = digitalRead(button1);
  bool btn2 = digitalRead(button2);
  Serial.println("{\"btn1\": \""+String(btn1)+"\",\"btn2\": \""+String(btn2)+"\"}");
  delay(500);
}
