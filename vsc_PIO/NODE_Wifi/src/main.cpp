#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>   //트렌지스터 2N2222 증폭겸 스위치 역할

#ifndef Wifi_ID
#define wifi_ID "LDH"
#define wifi_PW "ehdgml43"
// #define wifi_ID "iptime245"
// #define wifi_PW "pw101485"
#endif

#define _ledControlPin 5  //GPIO 5
String web_html ="<!DOCTYPE html> <html lang=\"en\"> <head> <meta charset=\"UTF-8\"> <meta name=\"viewport\" content=\"width=device-width, user-scalable=no\"> <title>Simple Mood Lamp</title> <link rel=\"stylesheet\" type=\"text/css\" href=\"./index.css\" /> </head> <body> <div class=\"out-box\"> <div class=\"content\"> <div> <input type=\"button\" value=\"\" id=\"onoff-button\" onclick=\"onClickOnOffButton()\"> </div> <div class=\"brightness-content\"> <div class=\"float-parent\"> <div class=\"float-left\"> <input type=\"range\" min=\"1\" max=\"100\" value=\"0\" id=\"brightness-slider\" oninput=\"onInputSlider()\" onchange=\"onChangeBrightness()\"> </div> <div class=\"float-left\" id=\"brightness-value\"/> </div> </div> </div> </div> </body> <script src=\"https://code.jquery.com/jquery-3.4.1.min.js\" integrity=\"sha256-CSXorXvZcTkaix6Yvo6HppcZGetbYMGWSFlBw8HfCJo=\" crossorigin=\"anonymous\"></script> <script src=\"index.js\"></script> </html>";
String web_css = "html body {\n    padding: 0;\n    margin: 0;\n    width: 100%;\n    height: 100%;\n \n}\n.out-box {\n    display: table;\n    position: fixed;\n    width: 100%;\n    height: 100%;\n \n}\n.content {\n    display: table-cell;\n    vertical-align: middle;\n    text-align: center;\n    width: 100%;\n}\n \n \n.brightness-content {\n    display: inline-block;\n    margin-top: 20px;\n}\n \n.float-left {\n    float: left\n}\n \n.float-parent {\n    height: 30px;\n    width: 100%;\n    text-align: center;\n    float: none;\n}\n \n#brightness-slider {\n    width: 200px;\n}\n \n#onoff-button {\n    background-color: gray; /* Green */\n    border: none;\n    color: white;\n    width: 150px;\n    height: 45px;\n    display: inline-block;\n    font-size: 20px;\n}";
String web_js = "var _brightnessSliderEle = null;\nvar _brightnessValueEle = null;\nvar _buttonEle = null;\nvar _state = null;\n \n \n$(document).ready(function() {\n    init();\n    onState();\n});\n \nfunction init() {\n    _brightnessSliderEle = $('#brightness-slider');\n    _brightnessValueEle = $('#brightness-value');\n    _buttonEle = $('#onoff-button');\n \n}\n \nfunction onState() {\n    $.get(\"http://192.168.10.39/state\").done(function (data)  {\n        setState(data);\n    }).fail(function (err) {\n \n    });\n}\n \nfunction setState(state) {\n    _state = state;\n    setBrightness(state.brightness);\n    setOnOff(state.isOn);\n}\n \nfunction onClickOnOffButton() {\n    _state.isOn = !_state.isOn;\n    var param = {isOn : _state.isOn };\n    $.post(\"http://192.168.10.39/onoff\",param ).done(function (data)  {\n        setState(data);\n    }).fail(function (err) {\n \n    });\n}\n \nfunction onChangeBrightness() {\n    _state.brightness = _brightnessSliderEle.val();\n    var param = {value : _state.brightness};\n    $.post(\"http://192.168.10.39/brightness\",param ).done(function (data)  {\n        setState(data);\n    }).fail(function (err) {\n \n    });\n}\n \nfunction onInputSlider() {\n    _brightnessValueEle.text(_brightnessSliderEle.val() + \"%\");\n}\n \nfunction setBrightness(value) {\n    _brightnessValueEle.text(value + \"%\");\n    _brightnessSliderEle.val(value);\n}\n \nfunction setOnOff(isOn) {\n    if(isOn === true) {\n        _buttonEle.val(\"ON\");\n        _buttonEle.css(\"background-color\",\"#4CAF50\")\n    } else {\n        _buttonEle.val(\"OFF\");\n        _buttonEle.css(\"background-color\",\"#f54b42\")\n    }\n}";


void onRequestState(void);
void onRequestOnOff(void);
void onRequestBrightness(void);
void setOnOff(void);
void setBright(void);
void onNotFound(void);
void onRequestIndexHTML(void);
void onRequestIndexJS(void);
void onRequestIndexCSS(void);

const char *_ssid = wifi_ID;
const char *_password = wifi_PW;

ESP8266WebServer _server(80);

bool _isOn = false;

int _brightness = 100;



void setup(void) {
  pinMode(_ledControlPin, OUTPUT);

  Serial.begin(115200);
  WiFi.mode(WIFI_STA);  //WIFI_STA, WIFI_AP, WIFI_AP_STA <- mode

  WiFi.begin(_ssid, _password);
  Serial.println("");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");

  Serial.println(_ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  _server.on("/", onRequestIndexHTML);
  _server.on("/index.css", onRequestIndexCSS);
  _server.on("/index.js", onRequestIndexJS);

  _server.on("/state", onRequestState);
  _server.on("/onoff", onRequestOnOff);
  _server.on("/brightness", onRequestBrightness);
  _server.onNotFound(onNotFound);




  _server.begin();
  Serial.println("HTTP_server started");
}


void loop(void) {
  _server.handleClient();
  // digitalWrite(_ledControlPin, HIGH);
  // delay(1000);
  // digitalWrite(_ledControlPin, LOW);
  // delay(1000);
}


void onRequestState(){
  String result = String("{\"isOn\":") + (_isOn ? "true": "false");
  result += String(",\"brightness\":") + _brightness + "}";

  _server.sendHeader("Access-Control-Allow-Origin" , "*");
  _server.send(200, "application/json", result);
}


void onRequestOnOff(){
  _isOn = String("true") == _server.arg("isOn");
  setOnOff();
  onRequestState();
}


void onRequestBrightness(){

  int value = (String("") + _server.arg("value")).toInt();

  if(value < 1) value = 1;
  else if (value > 100) value = 100;

  _brightness = value;

  setBright();
  onRequestState();

}


void setOnOff() {
  if(!_isOn) {
    digitalWrite(_ledControlPin, LOW);
  }
  else {
    setBright();
  }
}
 

void setBright() {
  if(!_isOn) return;
  // 1 에서 100사이의 값을 100에서 1024 사이의 값으로 치환
  int value = map(_brightness, 1,100, 100,1024);
  analogWrite(_ledControlPin, value);
}


void onNotFound(){
  String message = "404 File Not Found\n\n";
  message += "URI:";
  message += _server.uri();
  message += "\nMethod:";
  message += (_server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments";
  message += _server.args();
  message += "\n";

  for (uint8_t i = 0; i < _server.args(); i++)
  {
    message += "  " + _server.argName(i) + ":" + _server.arg(i) + "\n";
  }
  
  _server.send(404, "text/plain", message);

}

void onRequestIndexHTML() {
  _server.send(200, "text/html ", web_html); 
}
 
void onRequestIndexJS() {
  _server.send(200, "application/javascript ", web_js); 
}
 
void onRequestIndexCSS() {
  _server.send(200, "text/css", web_css); 
}


