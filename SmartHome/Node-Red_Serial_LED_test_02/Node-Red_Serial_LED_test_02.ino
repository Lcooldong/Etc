#define LED 23
#define BUTTON 18

bool pre_button_state = HIGH;

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
}

void loop() {
  
  bool now_button_state = digitalRead(BUTTON);

  // falling edge
  if (pre_button_state == HIGH && now_button_state == LOW){
    Serial.println('1');  // 이것을 받으면 1\r\n
    delay(200); // bounce 제거 (debounce)
  // rising edge
  }else if(pre_button_state == LOW && now_button_state == HIGH){
    Serial.println('0');
    delay(200);
  }
  pre_button_state = now_button_state;
  
  if(Serial.available())
  {
    char c = Serial.read();
    if (c == '0')
    {
      digitalWrite(LED, LOW);
    }else if(c=='1')
    {
      digitalWrite(LED, HIGH);
    }  
  }
}
