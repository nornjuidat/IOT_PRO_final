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

enum State { NORMAL, ALERT, NEUTRALIZED };
State systemState = NORMAL;

unsigned long lastBlink = 0;
bool ledState = false;

WebSocketsClient webSocket;

void IRAM_ATTR handleInterrupt() {
  panicPressed = true;
}

void sendStatus(const char* status) {
  StaticJsonDocument<128> doc;
  doc["device_id"] = "ESP01";
  doc["status"] = status;
  
  String jsonStr;
  serializeJson(doc, jsonStr);
  webSocket.sendTXT(jsonStr);
}

void webSocketEvent(WStype_t type, uint8_t* payload, size_t length) {
  if (type == WStype_TEXT) {
    String msg = String((char*)payload);
    if (msg.indexOf("cancel") >= 0 && systemState == ALERT) {
      systemState = NEUTRALIZED;
    }
  }
}


void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(buttonPin), handleInterrupt, FALLING);

  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  webSocket.begin(websocket_server, websocket_port, websocket_path);
  webSocket.onEvent(webSocketEvent);
  webSocket.setReconnectInterval(5000);

  sendStatus("NORMAL");
}
