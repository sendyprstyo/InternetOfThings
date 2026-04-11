#include "DHT.h"

// Tentukan pin yang digunakan (Contoh: GPIO 4)
#define DHTPIN 4     
#define DHTTYPE DHT11 

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200); // ESP32 standar menggunakan baud rate 115200
  dht.begin();
  Serial.println("DHT11 Sensor Reading started...");
}

void loop() {
  // Membaca kelembapan dan suhu [cite: 397]
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // Cek jika pembacaan gagal
  if (isnan(h) || isnan(t)) {
    Serial.println("Gagal membaca dari sensor DHT!");
    return;
  }

  // Menampilkan hasil di Serial Monitor [cite: 399]
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print("% | Temperature: ");
  Serial.print(t);
  Serial.println("°C");
  
  delay(2000); // Jeda pembacaan setiap 2 detik [cite: 399]
}