#include <WiFi.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>
#include "DHT.h"

#define DHTPIN 4
#define DHTTYPE DHT11
#define LDRPIN 34

DHT dht(DHTPIN, DHTTYPE);

const char* ssid     = "Asalole 4G";
const char* password = "21032006";

const char* ws_host = "192.168.18.226"; // IP laptop kamu
const int ws_port   = 8080;

WebSocketsClient webSocket;

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
    case WStype_CONNECTED:
      Serial.println("[WS] Terhubung ke server");
      break;
    case WStype_DISCONNECTED:
      Serial.println("[WS] Terputus");
      break;
  }
}

void setup() {
  Serial.begin(115200);
  dht.begin();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Terhubung");

  webSocket.begin(ws_host, ws_port, "/");
  webSocket.onEvent(webSocketEvent);
  webSocket.setReconnectInterval(5000);
}

void loop() {
  webSocket.loop();

  static unsigned long lastTime = 0;
  if (millis() - lastTime > 5000) {
    lastTime = millis();

    float h = dht.readHumidity();
    float t = dht.readTemperature();
    int ldr = analogRead(LDRPIN);

    if (isnan(h) || isnan(t)) {
      Serial.println("Gagal baca DHT!");
      return;
    }

    StaticJsonDocument<200> doc;
    doc["humidity"] = h;
    doc["suhu"] = t;
    doc["lumen"] = ldr;

    String json;
    serializeJson(doc, json);

    webSocket.sendTXT(json);
    Serial.println("Kirim: " + json);
  }
}