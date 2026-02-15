int count = 0;

void setup() {
  Serial.begin(115200);

}

void loop() {
  if(Serial.available()){
    String data = Serial.readStringUntil(0x0a); // 라인 피더
    Serial.print("[받음] : " + data);
    Serial.println(count);
    count++;
  }
}
