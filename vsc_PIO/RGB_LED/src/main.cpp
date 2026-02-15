#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);//1602

// Update these with values suitable for your network.
#define R_pin D7
#define G_pin D6
#define B_pin D5

const char* ssid = "iptime245";
const char* password = "pw101485";
const char* mqtt_server = "broker.mqtt-dashboard.com";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

byte hex2byte(String input);

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  lcd.setCursor(0,0); //첫번째 line
  lcd.print("NodeMCU Neopixel");
  lcd.setCursor(0,1); //두번째 line
  lcd.print(WiFi.localIP().toString());
}

void callback(char* topic, byte* payload, unsigned int length) {
  //Serial.print("Message arrived [");
  //Serial.print(topic);
  //Serial.print("] ");

  String mypayload;
  for (int i = 0; i < length; i++) {
    mypayload += (char)payload[i];
    //Serial.print((char)payload[i]);
  }
  //Serial.println();

  String mytopic = topic;

  Serial.print("NodeMCU TOPIC=");
  Serial.println(mytopic);
  Serial.print("NodeMCU PAYLOAD=");
  Serial.println(mypayload);

  if(mytopic == "nodemcu/rgb"){
    //나는 RGB무드등의 컬러를 바꾸겠다!
    // mypayload
    //#28A72F
    int r = hex2byte(mypayload.substring(1,3)); //bc
    int g = hex2byte(mypayload.substring(3,5)); //24
    int b = hex2byte(mypayload.substring(5,7)); //24

    //아두이노 analogWrite(0~255)
    //사물인터넷보드 anlogWirte(0~1023)
    r = map(r,0,255,1023,450);
    g = map(g,0,255,1023,0);
    b = map(b,0,255,1023,0);

    analogWrite(R_pin,r);
    analogWrite(G_pin,g);
    analogWrite(B_pin,b);

    client.publish("nodemcu/notice", "chaged");
  }
}

byte hex2byte(String input){
  //bc
  //input[0] //b
  //input[1] //c
  //0~9 A(10), B(11), C(12), D(13), E(14), F(15)

  int num1 = 0;
  int num2 = 0;
  if(input[0] >= '0' && input[0] <= '9'){
    //숫자인 경우
    num1 = input[0] - '0';
  }else if(input[0] >= 'A' && input[0] <= 'F'){
    num1 = input[0]-'A'+10;
  }

  if(input[1] >= '0' && input[1] <= '9'){
    //숫자인 경우
    num2 = input[1] - '0';
  }else if(input[1] >= 'A' && input[1] <= 'F'){
    num2 = input[1]-'A'+10;
  }

  return num1*16+num2;
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "NodeMCU Conneted");
      // ... and resubscribe
      //내가 구독하고자하는 topic을 등록하는 부분
      client.subscribe("nodemcu/rgb");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  
}