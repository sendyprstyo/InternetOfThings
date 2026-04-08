#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "Asalole 4G";
const char* password = "21032006";
// Sesuaikan IP Laptop dan nama file PHP untuk Suhu
const char* serverSuhu = "http://192.168.18.226/eslolin/simpan_suhu.php";

#define LM35_PIN 35

void setup() {
  Serial.begin(115200);
  delay(1000);
  WiFi.begin(ssid, password);
  Serial.print("Menghubungkan WiFi Suhu");
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
  Serial.println("\nConnected! IP: " + WiFi.localIP().toString());
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    // Membaca Suhu (Rata-rata 10x agar stabil)
    long reading = 0;
    for(int i=0; i<10; i++) { reading += analogRead(LM35_PIN); delay(10); }
    float avgADC = reading / 10.0;
    float millivolts = (avgADC / 4095.0) * 3300.0;
    float suhu = millivolts / 10.0;

    // Kirim ke Database
    HTTPClient http;
    http.begin(serverSuhu);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
    String postData = "suhu=" + String(suhu, 1);
    int httpCode = http.POST(postData);

    Serial.print("Suhu: "); Serial.print(suhu);
    Serial.print(" C | HTTP Code: "); Serial.println(httpCode);
    http.end();
  }
  delay(10000); // Kirim tiap 10 detik
}