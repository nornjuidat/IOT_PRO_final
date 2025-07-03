#include <ESP8266WiFi.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>


const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

const char* websocket_server = "10.0.0.14";
const int websocket_port = 3000;
const char* websocket_path = "/";

const int buttonPin = D1;
const int ledPin = D2;
const int buzzerPin = D3;

volatile bool panicPressed = false;
