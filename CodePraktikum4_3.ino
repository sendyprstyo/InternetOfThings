#include <WiFi.h>           
#include <HTTPClient.h>     
#include <WiFiClient.h>
#include "DHT.h"

#define DHTPIN 4          
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
WiFiClient wifiClient;

const char* ssid = "Asalole 4G";
const char* password = "21032006";

void setup() {
  Serial.begin(115200);
  dht.begin();
  
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if(WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    
    // Sesuaikan dengan IP Server/Laptop Anda [cite: 427, 438]
    // Ganti 192.168.1.10 dengan IP server tempat file sensordata.php berada
    String serverPath = "http://192.168.18.226/sensordata.php?humidity=" + String(h) + "&suhu=" + String(t);
    
    // Memulai koneksi HTTP
    http.begin(wifiClient, serverPath.c_str());
    
    int httpResponseCode = http.GET();
    
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("HTTP Response code: " + String(httpResponseCode));
      Serial.println("Server Response: " + response);
      if(httpResponseCode == 200) {
        Serial.println("Data Berhasil Tersimpan!");
      }
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    
    http.end(); 
  }
  
  delay(5000);
}