int count = 0;

void setup() {
  Serial.begin(115200);

}

void loop() {
  int sensor1 = random(0, 1023);
  int sensor2 = random(0, 1023);
  int sensor3 = random(0, 1023);

  String data = "{\"sensor1\":\""+String(sensor1)+"\", \"sensor2\":\""+String(sensor2)+"\", \"sensor3\":\""+String(sensor3)+"\"}";
  
  Serial.println(data);
  delay(1000);
}
