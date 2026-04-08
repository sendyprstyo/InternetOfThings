#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>

// --- 1. Konfigurasi WiFi & Database ---
const char* ssid = "Asalole 4G";
const char* password = "21032006";
const char* serverDB = "http://192.168.18.226/eslolin/simpan_evaluasi.php";

// --- 2. Pin Sensor ---
#define LDR_PIN 34
#define LM35_PIN 35

WebServer server(80);

// Variabel Global
float suhuC = 0.0;
int nilaiLDR = 0;

// --- 3. Tampilan Website (Grafik + jQuery Auto-load) ---
void handleRoot() {
  // Menggunakan Raw String Literal agar HTML lebih rapi dan mudah diedit
  String html = R"=====(
  <!DOCTYPE html>
  <html lang="id">
  <head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ESP32 Real-time Dashboard</title>
    <script src="https://code.jquery.com/jquery-3.6.0.min.js"></script>
    <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
    <link href="https://fonts.googleapis.com/css2?family=Poppins:wght@300;400;600&display=swap" rel="stylesheet">
    <style>
      * { box-sizing: border-box; }
      body { 
        font-family: 'Poppins', sans-serif; 
        background-color: #f4f7f6; 
        color: #2c3e50; 
        margin: 0; 
        padding: 20px; 
      }
      .dashboard-container { 
        max-width: 900px; 
        margin: 0 auto; 
      }
      .header { 
        text-align: center; 
        margin-bottom: 30px; 
      }
      .header h2 { 
        margin: 0; 
        font-size: 28px; 
        color: #34495e; 
      }
      .header p { 
        margin: 5px 0 0; 
        font-size: 14px; 
        color: #7f8c8d; 
      }
      /* Styling untuk Kartu Indikator */
      .cards { 
        display: flex; 
        justify-content: space-between; 
        gap: 20px; 
        margin-bottom: 25px; 
      }
      .card { 
        background: #ffffff; 
        border-radius: 16px; 
        padding: 25px; 
        flex: 1; 
        text-align: center; 
        box-shadow: 0 10px 20px rgba(0,0,0,0.05); 
        transition: transform 0.3s ease;
      }
      .card:hover {
        transform: translateY(-5px);
      }
      .card h3 { 
        margin: 0; 
        font-size: 16px; 
        color: #95a5a6; 
        font-weight: 400; 
      }
      .card .value { 
        font-size: 36px; 
        font-weight: 600; 
        margin-top: 10px; 
      }
      .val-suhu { color: #e74c3c; }
      .val-cahaya { color: #f39c12; }
      
      /* Styling untuk Container Grafik */
      .chart-container { 
        background: #ffffff; 
        padding: 25px; 
        border-radius: 16px; 
        box-shadow: 0 10px 20px rgba(0,0,0,0.05); 
        position: relative;
        height: 50vh;
        width: 100%;
      }
      /* Responsif untuk layar HP */
      @media (max-width: 600px) { 
        .cards { flex-direction: column; } 
        .chart-container { height: 40vh; }
      }
    </style>
  </head>
  <body>
    <div class="dashboard-container">
      <div class="header">
        <h2>Live Monitoring System</h2>
        <p>Dashboard Sensor Lolin D32</p>
      </div>
      
      <div class="cards">
        <div class="card">
          <h3>Suhu Saat Ini</h3>
          <div class="value val-suhu"><span id="txt-suhu">--</span> °C</div>
        </div>
        <div class="card">
          <h3>Intensitas Cahaya (ADC)</h3>
          <div class="value val-cahaya"><span id="txt-cahaya">--</span></div>
        </div>
      </div>

      <div class="chart-container">
        <canvas id="myChart"></canvas>
      </div>
    </div>

    <script>
      var ctx = document.getElementById('myChart').getContext('2d');
      var myChart = new Chart(ctx, {
        type: 'line',
        data: {
          labels: [],
          datasets: [
            {
              label: 'Suhu (°C)',
              borderColor: '#e74c3c',
              backgroundColor: 'rgba(231, 76, 60, 0.1)',
              data: [],
              fill: true,
              borderWidth: 2,
              pointRadius: 3,
              pointHoverRadius: 6,
              tension: 0.4
            },
            {
              label: 'Cahaya (ADC)',
              borderColor: '#f1c40f',
              backgroundColor: 'rgba(241, 196, 15, 0.1)',
              data: [],
              fill: true,
              borderWidth: 2,
              pointRadius: 3,
              pointHoverRadius: 6,
              tension: 0.4
            }
          ]
        },
        options: {
          responsive: true,
          maintainAspectRatio: false,
          plugins: {
            legend: { labels: { font: { family: 'Poppins' } } }
          },
          scales: {
            x: { grid: { display: false } },
            y: { beginAtZero: false, grid: { color: '#ecf0f1' } }
          }
        }
      });

      // Fungsi jQuery untuk ambil data tanpa refresh
      function updateChart() {
        $.getJSON('/data', function(json) {
          var now = new Date().toLocaleTimeString('id-ID', { hour12: false });
          
          // Update nilai di kartu UI
          $('#txt-suhu').text(json.temp);
          $('#txt-cahaya').text(json.ldr);

          // Update grafik
          if (myChart.data.labels.length > 15) { 
            myChart.data.labels.shift(); 
            myChart.data.datasets[0].data.shift(); 
            myChart.data.datasets[1].data.shift(); 
          }
          myChart.data.labels.push(now);
          myChart.data.datasets[0].data.push(json.temp);
          myChart.data.datasets[1].data.push(json.ldr);
          myChart.update();
        });
      }
      setInterval(updateChart, 2000); // Update tiap 2 detik
    </script>
  </body>
  </html>
  )=====";
  
  server.send(200, "text/html", html);
}

// Endpoint untuk menyediakan data JSON ke jQuery
void handleData() {
  String json = "{\"temp\":" + String(suhuC, 1) + ",\"ldr\":" + String(nilaiLDR) + "}";
  server.send(200, "application/json", json);
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
  
  Serial.println("\nConnected! IP: " + WiFi.localIP().toString());

  server.on("/", handleRoot);      // Menampilkan halaman grafik
  server.on("/data", handleData);  // Menyediakan data untuk jQuery
  server.begin();
}

void loop() {
  server.handleClient();

  // Membaca Sensor
  nilaiLDR = analogRead(LDR_PIN);
  long sum = 0;
  for(int i=0; i<10; i++) { sum += analogRead(LM35_PIN); delay(5); }
  suhuC = ( (sum/10.0) / 4095.0) * 3300.0 / 10.0;

  // Tetap kirim ke Database setiap 10 detik agar nomor 3 & 4 aman
  static unsigned long lastDB = 0;
  if (millis() - lastDB > 10000) {
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      http.begin(serverDB);
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      String postData = "suhu=" + String(suhuC, 1) + "&cahaya=" + String(nilaiLDR);
      http.POST(postData);
      http.end();
    }
    lastDB = millis();
  }
}