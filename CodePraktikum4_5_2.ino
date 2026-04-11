#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClient.h>
#include "DHT.h"

// --- KONFIGURASI SENSOR ---
#define DHTPIN 4          
#define LDRPIN 34         
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

// --- KONFIGURASI WIFI ---
const char* ssid = "Asalole 4G";
const char* password = "21032006";

// --- KONFIGURASI SERVER LARAGON ---
// Ganti dengan IPv4 laptop Anda (Cek via CMD: ipconfig)
// Pastikan Laragon sudah "Start All"
String serverIP = "192.168.18.226"; 
String serverPath = "http://192.168.18.226/data_dashboard.php";

void setup() {
  Serial.begin(115200);
  dht.begin();

  WiFi.begin(ssid, password);
  Serial.print("Menghubungkan ke WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Terhubung!");
  Serial.print("IP Address ESP32: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int lumen = analogRead(LDRPIN); 

  if (isnan(h) || isnan(t)) {
    Serial.println("Gagal membaca sensor DHT11!");
    return;
  }

  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;

    // Laragon biasanya lebih sensitif terhadap format URL
    String urlData = serverPath + "?lumen=" + String(lumen) + 
                     "&suhu=" + String(t) + 
                     "&humidity=" + String(h);

    Serial.println("Mengirim data ke Laragon...");
    
    http.begin(client, urlData);
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      String payload = http.getString();
      Serial.print("Respon Laragon [");
      Serial.print(httpResponseCode);
      Serial.print("]: ");
      Serial.println(payload);
    } else {
      Serial.print("Gagal! Error: ");
      Serial.println(http.errorToString(httpResponseCode).c_str());
    }
    http.end(); 
  }

  delay(5000);
}