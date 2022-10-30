#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <DHT.h>
#include <ESP8266HTTPClient.h>

#include <WiFiClient.h>
#define pinDHT 2
#define DHTTYPE DHT11
DHT dht (pinDHT,DHTTYPE);
ESP8266WiFiMulti WiFiMulti;
String serverName = "http://192.168.25.54/";
String Humidity="humidity/";
String Temp="temp/";
String Control="control";
int humidity=0;
int temp=0
char link;
void setup() {

  Serial.begin(115200);
  // Serial.setDebugOutput(true);

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

}
void loop(){
  int h=dht.readHumidity();
  int t=dht.readTemperature();
  if((WiFiMulti.run() == WL_COMMECTED)){
    WiFiClient client;

   
   
      HTTPClient http;
      Serial.print("[HTTP] begin...\n");
      String serverh =serverName + Humidity +h;
      delay(100);
      if (http.begin(client, serverh.c_str())) {  // HTTP


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
        Serial.println(serverh.c_str());
        Serial.printf("[h] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }
    http.end();  
  }else {
    Serial.println(serverh.c_str());
    Serial.printf("[h} Unable to connect\n");
  }
      
  }
  if((WiFiMulti.run() == WL_COMMECTED)){
    WiFiClient client;

   
   
      HTTPClient http;
      Serial.print("[HTTP] begin...\n");
      String servert =serverName + temp +t;
      delay(100);
      if (http.begin(client, serverh.t_str())) {  // HTTP


        Serial.print("[t] GET...\n");
      // start connection and send HTTP header
        int httpCode = http.GET();

      // httpCode will be negative on error
        if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
          Serial.printf("[t] GET... code: %d\n", httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString();
          Serial.println(payload);
        }
      } else {
        Serial.println(servert.c_str());
        Serial.printf("[h] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }
    http.end();  
  }else {
    Serial.println(servert.c_str());
    Serial.printf("[t} Unable to connect\n");
  }
      
  }
}
