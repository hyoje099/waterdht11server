#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>
#include <DHT11.h>
#include<DHT.h>
#include <WiFiClient.h>
#define pinDHT 2
#define DHTTYPE DHT11
DHT dht (pinDHT,DHTTYPE);
