#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

WiFiClient wifiClient;

// Konfigurasi WiFi
const char* ssid = "Asalole 4G";
const char* password = "21032006";

// Definisi PIN GPIO untuk 1 LED saja
const int pinLED = 12; 

void setup() {
  Serial.begin(115200);
  pinMode(pinLED, OUTPUT);
  
  // Memulai Koneksi WiFi
  WiFi.begin(ssid, password);
  Serial.print("Menghubungkan WiFi");
  while (WiFi.status() != WL_CONNECTED) { 
    delay(500); 
    Serial.print("."); 
  }
  Serial.println("\nWiFi Terkoneksi!");
}

void loop() {
  if(WiFi.status() == WL_CONNECTED){
    HTTPClient http;
    
    // URL API baca data
    http.begin(wifiClient, "http://192.168.18.226/eslolin/api_baca.php");
    
    int httpCode = http.GET();
    if(httpCode == HTTP_CODE_OK){
      String payload = http.getString();
      
      // Alokasi memori JSON
      StaticJsonDocument<256> doc; 
      DeserializationError error = deserializeJson(doc, payload);
      
      if (!error) {
        // Mengambil status khusus untuk "Lampu" sesuai soal No. 1
        String statusLampu = doc["Lampu"]["statusSensor"]; 
        
        Serial.print("Status Lampu: ");
        Serial.println(statusLampu);
        
        // Eksekusi Hardware
        // Jika status "1" maka HIGH (nyala), jika selain itu maka LOW (mati)
        if (statusLampu == "1") {
          digitalWrite(pinLED, HIGH);
        } else {
          digitalWrite(pinLED, LOW);
        }
      } else {
        Serial.print("Gagal parse JSON: ");
        Serial.println(error.f_str());
      }
    } else {
      Serial.printf("HTTP Error: %d\n", httpCode);
    }
    http.end();
  }
  
  delay(3000); // Sinkronisasi setiap 3 detik
}