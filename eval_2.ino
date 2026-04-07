#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>

// ===== WIFI =====
const char* ssid = "Asalole 4G";
const char* password = "21032006";

String serverUrl = "http://192.168.18.226/eslolin/suhu.php";

// ===== PIN =====
#define LDR_PIN 34
#define LM35_PIN 35

WebServer server(80);

// ===== VARIABEL =====
int nilaiLDR = 0;
String statusCahaya = "Membaca...";
float suhuC = 0.0;
String kategoriSuhu = "Membaca...";

// ===== WEB UI =====
void handleRoot() {
  String warnaCahaya = "#3498db";
  if (statusCahaya == "Gelap") warnaCahaya = "#2c3e50";
  else if (statusCahaya == "Redup") warnaCahaya = "#f39c12";
  else warnaCahaya = "#2ecc71";

  String warnaSuhu = "#3498db";
  if (kategoriSuhu == "DINGIN") warnaSuhu = "#00bcd4";
  else if (kategoriSuhu == "NORMAL") warnaSuhu = "#2ecc71";
  else warnaSuhu = "#e74c3c";

  String html = R"====(
  <!DOCTYPE html>
  <html>
  <head>
    <title>Smart IoT Dashboard</title>
    <meta http-equiv='refresh' content='2'>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
      body {
        margin: 0;
        font-family: 'Segoe UI', sans-serif;
        background: linear-gradient(135deg, #667eea, #764ba2);
        color: white;
        text-align: center;
      }

      h2 {
        margin-top: 20px;
        font-weight: 600;
      }

      .container {
        display: flex;
        flex-wrap: wrap;
        justify-content: center;
        gap: 20px;
        padding: 20px;
      }

      .card {
        width: 300px;
        padding: 20px;
        border-radius: 20px;
        backdrop-filter: blur(10px);
        background: rgba(255,255,255,0.1);
        box-shadow: 0 8px 32px rgba(0,0,0,0.3);
        transition: 0.3s;
      }

      .card:hover {
        transform: translateY(-10px) scale(1.03);
      }

      .value {
        font-size: 40px;
        font-weight: bold;
        margin: 10px 0;
      }

      .label {
        font-size: 14px;
        opacity: 0.8;
      }

      .status {
        margin-top: 10px;
        padding: 8px 15px;
        border-radius: 50px;
        font-weight: bold;
        display: inline-block;
      }

      footer {
        margin-top: 20px;
        font-size: 12px;
        opacity: 0.7;
      }
    </style>
  </head>

  <body>
    <h2>🌐 Smart Monitoring Dashboard</h2>

    <div class="container">

      <div class="card">
        <div class="label">🌙 Intensitas Cahaya</div>
        <div class="value">)====";

  html += statusCahaya;

  html += R"====(</div>
        <div class="status" style="background:)====";

  html += warnaCahaya;

  html += R"====(;">ADC: )====";

  html += String(nilaiLDR);

  html += R"====(</div>
      </div>

      <div class="card">
        <div class="label">🌡 Suhu Ruangan</div>
        <div class="value">)====";

  html += String(suhuC, 1);

  html += R"====(&deg;C</div>
        <div class="status" style="background:)====";

  html += warnaSuhu;

  html += R"====(;">)====";

  html += kategoriSuhu;

  html += R"====(</div>
      </div>

    </div>

    <footer>ESP32 IoT Monitoring • Auto Refresh 2s</footer>
  </body>
  </html>
  )====";

  server.send(200, "text/html", html);
}

// ===== SETUP =====
void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("\n--- Memulai ESP32 Lolin D32 ---");

  WiFi.begin(ssid, password);
  Serial.print("Menghubungkan ke WiFi...");

  int timeout = 0;
  while (WiFi.status() != WL_CONNECTED && timeout < 20) {
    delay(500);
    Serial.print(".");
    timeout++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n[BERHASIL] WiFi Terhubung!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\n[GAGAL] Cek Hotspot!");
  }

  server.on("/", handleRoot);
  server.begin();
}

// ===== LOOP =====
void loop() {
  server.handleClient();

  // ===== LDR =====
  nilaiLDR = analogRead(LDR_PIN);
  if (nilaiLDR < 800) statusCahaya = "Gelap";
  else if (nilaiLDR < 2000) statusCahaya = "Redup";
  else statusCahaya = "Terang";

  // ===== LM35 =====
  int reading = analogRead(LM35_PIN);
  float voltage = (reading / 4095.0) * 3300.0;
  suhuC = voltage / 10.0;

  if (suhuC < 22) kategoriSuhu = "DINGIN";
  else if (suhuC <= 30) kategoriSuhu = "NORMAL";
  else kategoriSuhu = "PANAS";

  // ===== SERIAL LOG =====
  static unsigned long lastLog = 0;
  if (millis() - lastLog > 2000) {
    Serial.printf("IP: %s | Suhu: %.1f°C | Cahaya: %s\n",
      WiFi.localIP().toString().c_str(),
      suhuC,
      statusCahaya.c_str()
    );
    lastLog = millis();
  }
}