#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>

// Konfigurasi WiFi
const char* ssid = "Asalole 4G";
const char* password = "21032006";
String serverUrl = "http://192.168.18.226/eslolin/cahaya.php";

#define LDR_PIN 34

// Inisialisasi Web Server pada port 80
WebServer server(80);

// Variabel Global untuk menyimpan data terakhir
int nilaiLDR = 0;
String kategori = "";

// Fungsi untuk menangani tampilan di Browser
void handleRoot() {
  String html = "<!DOCTYPE html><html>";
  html += "<head><title>Evaluasi 3.7 - Sensoring</title>";
  html += "<meta http-equiv='refresh' content='2'>"; // Auto-refresh tiap 2 detik
  html += "<style>body { font-family: Arial; text-align: center; background-color: #f4f4f4; }";
  html += ".container { background: white; width: 400px; margin: 50px auto; padding: 20px; border-radius: 10px; box-shadow: 0 0 10px rgba(0,0,0,0.1); }";
  html += ".data { font-size: 24px; font-weight: bold; color: blue; }";
  html += ".label { color: #7f8c8d; margin-top: 10px; }</style></head>";
  html += "<body><div class='container'>";
  html += "<h2>Evaluasi 3.7 - Sensoring</h2><hr>";
  html += "<h1>Hasil Monitoring Cahaya</h1>";
  html += "<h3>Kondisi Ruangan saat ini: <span class='data'>" + kategori + "</span></h3>";
  html += "<p>(Nilai ADC: " + String(nilaiLDR) + ")</p>";
  html += "</div></body></html>";
  
  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);
  
  // Menghubungkan WiFi
  WiFi.begin(ssid, password);
  Serial.print("Menghubungkan ke WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  // Menampilkan IP Address di Serial Monitor
  Serial.println("\nWiFi Connected!");
  Serial.print("IP Address ESP32: ");
  Serial.println(WiFi.localIP()); // <-- IP inilah yang nanti dipaste ke browser

  // Menentukan route server
  server.on("/", handleRoot);
  server.begin();
  Serial.println("Web Server Berjalan...");
}

void loop() {
  // Menangani request dari browser
  server.handleClient();

  // Membaca Sensor LDR
  nilaiLDR = analogRead(LDR_PIN);

  // Logika Kategori Kecerahan
  if (nilaiLDR < 500) kategori = "Gelap";
  else if (nilaiLDR < 1500) kategori = "Redup";
  else if (nilaiLDR < 3000) kategori = "Cerah";
  else kategori = "Terang";

  // Tetap mengirim ke database PHP (Opsional)
  static unsigned long lastTime = 0;
  if (millis() - lastTime > 5000) { // Kirim tiap 5 detik agar tidak berat
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      http.begin(serverUrl + "?status=" + kategori);
      int httpCode = http.GET();
      if (httpCode > 0) Serial.println("Data terkirim ke server lokal.");
      http.end();
    }
    lastTime = millis();
  }
}