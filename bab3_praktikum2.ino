#include <WiFi.h>
#include <HTTPClient.h>

// Variabel global
const char* ssid = "Asalole 4G";
const char* password = "21032006";
int pinLED =5;
#define ON HIGH
#define OFF LOW
//#define USE_SERIAL Serial

void setup() {
  // Konfigurasi Modul
  Serial.begin(115200);
  
  // Connect to WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  for(uint8_t t = 4; t > 0; t--) {
        Serial.printf("[SETUP] WAIT %d...\n", t);
        Serial.flush();
        delay(1000);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());

  // --- End of Connect wifi

  // Deklarasi PIN GPIO
  pinMode(pinLED, OUTPUT);
}

void loop() {
  if(WiFi.status() == WL_CONNECTED){
    HTTPClient http;

    Serial.print("[HTTP] begin...\n");
    
    // IP menuju ke server web
    http.begin("http://192.168.18.226/eslolin/sensorsingle.php");
    Serial.print("[HTTP] GET...\n");
    
    int httpCode = http.GET();
    Serial.print(httpCode);
    
    if(httpCode > 0){
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);
      
      // file found at server
      //If(200 == 200)
      if(httpCode == HTTP_CODE_OK) {
        String status = http.getString();
        
        Serial.println("LED 1 --> " + status);
        if(status.indexOf("0") != -1){
          //relay = OFF;
          digitalWrite(pinLED, OFF);
          Serial.println("LED Posisi OFF");
        } else {
          digitalWrite(pinLED, ON);
          Serial.println("LED Posisi ON");
        }
      }
    } else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    
    http.end();
    delay(50);      
    
  } else {
    Serial.println("Delay...");  
  }
  
  delay(1000);  
}