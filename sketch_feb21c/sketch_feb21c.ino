#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <SoftwareSerial.h> 
#define TX D6 // arduino softSerial RX -> NodeMcu D6(TX)
#define RX D7 // arduino softSerial TX -> NodeMcu D7(RX) 
SoftwareSerial arduSerial(RX, TX); // (RX, TX)

char float1[10] = {0,}; // atof()함수를 위해 char 자료형 배열을 10바이트 선언

#define LED LED_BUILTIN
const char* ssid = "hanul201";
const char* password = "hanul201";

ESP8266WebServer server(80);
// 가변저항 이용.

void handleRoot() {
  Serial.println("You called root page");

  // 보여줄 웹페이지와 스크립트 생성.
  String s = "<!DOCTYPE html><html><head><meta charset='UTF-8'></head><body>";
  s += "<div><h1>NodeMCU_13</h1>";
  s += "<button type='button' onclick='sendData(1)'>LED ON</button>";
  s += "<button type='button' onclick='sendData(0)'>LED OFF</button><BR></div>";
  s += "<div>ADC Value is : <span id='ADCValue'>0</span><br>";
  s += "LED State is : <span id='LEDState'>NA</span></div>";
  s += "<script src='https://code.jquery.com/jquery-3.6.0.min.js'></script>";
  s += "<script>";
  s += " function sendGet(param) {";
  s += "$.ajax({";
  s += "type : 'get',";
  s += "url : 'http://192.168.0.11:8099/node?led=' + param,";
  s += "success : function(result, status, xhr) {";
  s += "console.log(result);";
  s += "},});}";
  s += "function sendData(led) {";
  s += "$.ajax({  type : 'get',";
  s += "        url : '/setLED?LEDstate=' + led,";
  s += "    success : function(result, status, xhr) { ";
  s += "  console.log(result);";
  s += "          $('#LEDState').html(result);";
  s += "      sendGet(result);";
  s += "},      });   }";
  s += "setInterval(function() { ";
  s += "getData();";
  s += "}, 2000);";
  s += "function getData() { ";
  s += "$.ajax({  type : 'get',";
  s += "url : '/readADC',";
  s += "success : function(result, status, xhr) { ";
  s += "          console.log(result);";
  s += "          $('#ADCValue').html(result);";
  s += "sendData( $('#ADCValue').text())";  
  s += "  },      });";
  s += "}</script><br></body></html>";

  server.send(200, "text/html", s);// 웹 브라우저에 표시.
}

void handleADC() {
 if (arduSerial.available() > 0) {
    for (int i = 0; i < 10; i++) { // 시리얼 버퍼내 10바이트의 데이터를 float1 배열에 저장
      uint8_t temp = arduSerial.read();
      float1[i] = temp;
    }
  }
  else {
    float val = atof(float1);  // 문자 배열을 실수로 변환하는 함수, 배열 자료형은 char
    String adcValue = String(val);
    Serial.print(val);
    Serial.println();
    server.send(200, "text/plane", adcValue);    
    arduSerial.write(0);
    delay(1000);  // 응답 대기 시간
   
  }
   
  
}

// 잘못된 접근시 안내 메세지 생성.
void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void handleLED() {
  String ledState = "OFF";
  String t_state = server.arg("LEDstate");
  //Refer  xhttp.open("GET", "setLED?LEDstate="+led, true);
  Serial.print(t_state + " ");
  if (t_state == "1")
  {
    digitalWrite(LED, LOW); //LED ON
    ledState = "ON"; //Feedback parameter
    Serial.println("led_on");
  }
  else
  {
    digitalWrite(LED, HIGH); //LED OFF
    ledState = "OFF"; //Feedback parameter
    Serial.println("led_off");
  }

  server.send(200, "text/plane", ledState); //Send web page
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  arduSerial.begin(9600);
  pinMode(LED, OUTPUT);
  pinMode(A0, INPUT);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/setLED", handleLED);
  server.on("/readADC", handleADC);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}