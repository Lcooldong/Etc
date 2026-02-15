#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

ESP8266WebServer server(80);

int led = 14;

void sendHtml(String url){

  String html = "";

  html += "<html><script src=\"http://www.w3schools.com/lib/w3data.js\"></script>";
  html += "<body><div w3-include-html=\"" + url + "\"></div>";
  html += "<script>w3IncludeHTML();</script></body></html>";
  server.send(200, "text/html", html);

}

void setup(void){

  pinMode(led, OUTPUT);

  digitalWrite(led, LOW);

  Serial.begin(115200);



  // 연결 완료 까지 대기

  while (WiFi.status() != WL_CONNECTED) {

    delay(500);

    Serial.print(".");

  }

  Serial.println("");

  Serial.print("Connected to ");

  Serial.println(WiFi.SSID());

  Serial.print("IP address: ");

  Serial.println(WiFi.localIP());



  if (MDNS.begin("esp8266")) {

    Serial.println("MDNS responder started");

  }



  //접속별 동작 설정



  server.on("/", [](){

    sendHtml("https://raw.githubusercontent.com/keepworking/ESP8266-PANNEL/master/index.html");

  });



  server.on("/on", [](){//ON 으로 접속시 LED를 켭니다.

    digitalWrite(led, HIGH);

    sendHtml("https://raw.githubusercontent.com/keepworking/ESP8266-PANNEL/master/index.html");

  });



  server.on("/off", [](){//OFF 으로 접속시 LED를 끕니다.

    digitalWrite(led, LOW);

    sendHtml("https://raw.githubusercontent.com/keepworking/ESP8266-PANNEL/master/index.html");

  });

  

  //서버 사용을 시작합니다.



  server.begin();

  Serial.println("HTTP server started");

}



void loop(void){

  server.handleClient();//클라이언트 접속을 관리합니다.

}