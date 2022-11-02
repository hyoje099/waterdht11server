#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <DHT11.h>
#include <DHT.h>
#include <ESP8266HTTPClient.h>

#include <WiFiClient.h>
#define pinDHT 0
#define DHTTYPE DHT11
DHT dht (pinDHT,DHTTYPE);
ESP8266WiFiMulti WiFiMulti;
String serverName = "http://192.168.25.59/";
String Humidity="humidity/";
String Temp="/temp/";
String Control="control";
const int B_IA = 13; 
const int B_IB = 12; 
const int A_IA = 4; 
const int A_IB = 5; 
int TrigPin = 16;
int EchoPin = 15;
char link;
long duration, distance;
///////////장애물 확인 및 회피 방향 결정///////////
void Obstacle_Check() {
  int val = random(2);
  Distance_Measurement();
  delay(50);
  
  Serial.println(distance);

  while (distance < 300) {
    if (distance < 150) {
      backward();
      delay(800);
      stopCar();
      delay(50);
      Distance_Measurement();
      delay(100);
    }
    else {
      if (val == 0) {
        right();
        delay(400);
      }
      else if (val == 1) {
        left();
        delay(400);
      }
      Distance_Measurement();
      delay(100);
    }
  }
}


////////거리감지///////////
void Distance_Measurement() {
  digitalWrite(TrigPin, LOW);
  delay(2);
  digitalWrite(TrigPin, HIGH);  // trigPin에서 초음파 발생(echoPin도 HIGH)
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);
  duration = pulseIn(EchoPin, HIGH);    // echoPin 이 HIGH를 유지한 시간을 저장 한다.
  distance = ((float)(340 * duration) / 1000) / 2;
  delay(50);
}
void forward(){
  digitalWrite(B_IA, HIGH);//앞으로
  digitalWrite(B_IB, LOW);
  digitalWrite(A_IA, HIGH);//앞으로
  digitalWrite(A_IB, LOW);
}
void backward(){
  digitalWrite(B_IA, LOW);//뒤로
  digitalWrite(B_IB, HIGH);
  digitalWrite(A_IA, LOW);//뒤로
  digitalWrite(A_IB, HIGH);
}
void stopCar(){
  digitalWrite(B_IA, LOW);//멈춤
  digitalWrite(B_IB, LOW);
  digitalWrite(A_IA, LOW);//멈춤
  digitalWrite(A_IB, LOW);
}
void right(){
  digitalWrite(A_IA, HIGH);//앞으로
  digitalWrite(A_IB, LOW);
  digitalWrite(B_IA, LOW);//뒤로
  digitalWrite(B_IB, HIGH);
}
void left(){
  digitalWrite(A_IA, LOW);//뒤로
  digitalWrite(A_IB, HIGH);
  digitalWrite(B_IA, HIGH);//앞으로
  digitalWrite(B_IB, LOW);
}
void setup() {

  Serial.begin(115200);
  // Serial.setDebugOutput(true);
  pinMode(EchoPin, INPUT);   // EchoPin 입력
  pinMode(TrigPin, OUTPUT);  // TrigPin 출력
  pinMode(B_IA, OUTPUT);
  pinMode(B_IB, OUTPUT);
  pinMode(A_IA, OUTPUT);
  pinMode(A_IB, OUTPUT);
  Serial.println();
  Serial.println();
  Serial.println();

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("%d second\n", t);
    Serial.flush();
    delay(1000);
  }
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("SK_WiFi4FED", "1402033324");
  //WiFiMulti.addAP("UCA", "0527080001");
}
void loop() {
  int h=dht.readHumidity();
  int t=dht.readTemperature();
  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {

    WiFiClient client;

      HTTPClient http;

      Serial.print("[HTTP] begin...\n");
      delay(100);
      String serverPath = serverName + Humidity + h + Temp + t;
      if (http.begin(client, serverPath.c_str())) {  // HTTP


        Serial.print("[HTTP] GET...\n");
      // start connection and send HTTP header
        int httpCode = http.GET();

      // httpCode will be negative on error
        if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
          Serial.printf("[HTTP] GET... code: %d\n", httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString();
          Serial.println(payload);
        }
      } else {
        Serial.println(serverPath.c_str());
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }

      http.end();
    } else {
      Serial.println(serverPath.c_str());
      Serial.printf("[HTTP} Unable to connect\n");
    }
    
    

    
    
  }
   if(WiFi.status() == WL_CONNECTED){//자동차의 모드를 선택하는 부분이다.
    WiFiClient client;
        HTTPClient http;
        String getRequestURL = "http://192.168.25.59/control";
        
        
        http.begin(client,getRequestURL);

        int httpCode = http. GET();
        if(httpCode>0){
            String payload =http.getString();
            Serial.println(payload);
            
            int i=0;
            if(payload.equals("0"){
              stopCar();
            }else if(payload.equals("1"){
              forward();
            }else if(payload.equals("2"){
              right();
            }else if(payload.equals("3"){
              left();
            }else if(payload.equals("4"){
              backward();
            }else if(payload.equals("5"){
              forward();
              delay(50);
              Obstacle_Check();
            }
            
            
        
        
        http.end();
        

  delay(1000);
}
}
