#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "Asalole 4G";
const char* password = "21032006";

int pinLED = 12;

void setup() {
  Serial.begin(115200);
  pinMode(pinLED, OUTPUT);

  WiFi.begin(ssid, password);
  Serial.print("Connecting");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    String serverPath = "http://192.168.18.226/eslolin/sensormulti-json.php";
    http.begin(serverPath);

    int httpCode = http.GET();

    if (httpCode == 200) {
      String payload = http.getString();
      Serial.println(payload);

      StaticJsonDocument<256> doc;
      deserializeJson(doc, payload);

      int status = doc["statusSensor"];

      if (status == 1) {
        digitalWrite(pinLED, HIGH);
      } else {
        digitalWrite(pinLED, LOW);
      }
    }

    http.end();
  }

  delay(3000);
}