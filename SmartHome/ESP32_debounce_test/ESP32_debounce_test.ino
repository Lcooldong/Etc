#define LED 5
#define Button 23

boolean last = HIGH;
boolean current = HIGH;
boolean ledOn = LOW;

boolean debounce(boolean last)
{
  boolean current = digitalRead(Button);
  if(last != current)
  {
    delay(5);
    current = digitalRead(Button);  
  }
  return current;
}


void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  pinMode(Button, INPUT_PULLUP);    // HIGH
  Serial.println(ledOn); 
}

void loop() {
  boolean current = debounce(last);
  // !last && current
  if(last == HIGH && current == LOW)  //PULLUP 이라서 초기값이 HIGH 누른 값이 LOW
  {
    ledOn = !ledOn;
    Serial.println("작동함");
    Serial.println(ledOn);
  }
  last = current;

  digitalWrite(LED, ledOn);
}
