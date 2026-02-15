#include <Arduino.h>

#define R_pin 7
#define G_pin 8
#define B_pin 9

void setRGB(int, int, int);

void setup() {
  Serial.begin(115200);
  analogWrite(R_pin, 1023);  // 공통 양이라서 0에 가까울 수록 켜짐
  analogWrite(G_pin, 1023);
  analogWrite(B_pin, 1023);

}

void loop() {

  setRGB(255,0,0);  //red
  delay(500);
  setRGB(0,255,0);  //green
  delay(500);
  setRGB(0,0,255);  //blue
  delay(500);
  setRGB(255,255,0);  //yellow
  delay(500);
  setRGB(0,255,255);  //cyan
  delay(500);
  setRGB(255,0,255);  //magenta
  delay(500);

}


void setRGB(int R_value, int G_value, int B_value){

//Anode
  R_value = map(R_value, 0, 255, 1023, 512);  //빨강이 너무 강해서 반으로 줄임
  G_value = map(G_value, 0, 255, 1023, 0);
  B_value = map(B_value, 0, 255, 1023, 0);

  analogWrite(R_pin, R_value);  // 공통 양이라서 0에 가까울 수록 켜짐
  analogWrite(G_pin, G_value);
  analogWrite(B_pin, B_value);

  Serial.print("R :");
  Serial.print(R_value);
  Serial.print("\t");
  Serial.print("G :");
  Serial.print(G_value);
  Serial.print("\t");
  Serial.print("B :");
  Serial.println(B_value);
  delay(1000);

}