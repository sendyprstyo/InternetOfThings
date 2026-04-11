#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"

// Konfigurasi Pin
#define DHTPIN 4
#define LDRPIN 34
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "Asalole 4G";
const char* password = "21032006";

// Ganti dengan IP Laptop kamu dan port Express (default 3000)
String serverUrl = "http://192.168.18.226:3000/api/sensor";

void setup() {
  Serial.begin(115200);
  dht.begin();
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Terhubung!");
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int lumen = analogRead(LDRPIN);

  if (isnan(h) || isnan(t)) return;

  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;

    http.begin(client, serverUrl);
    
    // Menentukan format data yang dikirim adalah JSON
    http.addHeader("Content-Type", "application/json");

    // Membuat body pesan dalam format JSON
    String httpRequestData = "{\"suhu\":\"" + String(t) + "\",\"kelembapan\":\"" + String(h) + "\",\"lumen\":\"" + String(lumen) + "\"}";

    int httpResponseCode = http.POST(httpRequestData);

    if (httpResponseCode > 0) {
      Serial.print("Data Terkirim, Respon: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.print("Error: ");
      Serial.println(http.errorToString(httpResponseCode).c_str());
    }
    http.end();
  }
  delay(5000);
}