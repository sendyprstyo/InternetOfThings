#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "Asalole 4G";
const char* password = "21032006";
const char* serverName = "http://192.168.18.226/simpan_lumen.php";

const int ldrPin = 34; // Menggunakan Pin Analog 33 sesuai sebelumnya

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    int nilaiLumen = analogRead(ldrPin); // Membaca intensitas cahaya

    HTTPClient http;
    http.begin(serverName);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    String httpRequestData = "lumen=" + String(nilaiLumen);
    int httpResponseCode = http.POST(httpRequestData);

    Serial.println("Lumen Terkirim: " + String(nilaiLumen));
    http.end();
  }
  delay(10000); // Kirim setiap 10 detik
}