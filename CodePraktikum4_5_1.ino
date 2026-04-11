#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClient.h>
#include "DHT.h"

#define DHTPIN 4
#define DHTTYPE DHT11
#define LDRPIN 34   // pin analog untuk LDR

DHT dht(DHTPIN, DHTTYPE);
WiFiClient wifiClient;

const char* ssid = "Asalole 4G";
const char* password = "21032006";

void setup() {
  Serial.begin(115200);
  dht.begin();

  WiFi.begin(ssid, password);
  Serial.print("Connecting WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected");
  Serial.print("IP ESP32: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int ldr = analogRead(LDRPIN);

  // cek sensor
  if (isnan(h) || isnan(t)) {
    Serial.println("Sensor gagal dibaca!");
    return;
  }

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // ⚠️ SESUAIKAN URL INI!
    String url = "http://192.168.18.226/input_data.php?lumen=" + String(ldr) +
                 "&suhu=" + String(t) + "&humidity=" + String(h);

    Serial.println("Kirim ke:");
    Serial.println(url);

    http.begin(wifiClient, url);
    int httpCode = http.GET();

    Serial.print("HTTP Code: ");
    Serial.println(httpCode);

    if (httpCode > 0) {
      String response = http.getString();
      Serial.println("Response:");
      Serial.println(response);
    } else {
      Serial.println("Gagal kirim data!");
    }

    http.end();
  }

  delay(5000); // kirim tiap 5 detik
}