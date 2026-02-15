#include <Arduino.h>

const int led = 5;  //GPIO 5

void setup() {
  Serial.begin(9600);
  pinMode(led, OUTPUT);
}

void loop() {
  digitalWrite(led, HIGH);
  Serial.print("ON");
  delay(1000);
  digitalWrite(led, LOW);
  Serial.print("OFF");
  delay(500);
}