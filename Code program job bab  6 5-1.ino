#include "DHT.h"

#define DHTPIN 4
#define DHTTYPE DHT11

#define ENABLE_USER_AUTH
#define ENABLE_DATABASE

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <FirebaseClient.h>

void processData(AsyncResult &aResult);

// WIFI
#define WIFI_SSID "Asalole 4G"
#define WIFI_PASSWORD "21032006"

// FIREBASE
#define API_KEY "AIzaSyC634_SkUiV00zzPPhfTr6A1yR4VSQCsDk"
#define USER_EMAIL "sendy@gmail.com"
#define USER_PASSWORD "12345678"
#define DATABASE_URL "https://iot-sendy-db-default-rtdb.asia-southeast1.firebasedatabase.app"

// LED
const int led1 = 12;
#define ON HIGH
#define OFF LOW

// AUTH
UserAuth user_auth(API_KEY, USER_EMAIL, USER_PASSWORD);
FirebaseApp app;

// SSL CLIENT
WiFiClientSecure ssl_client1, ssl_client2;
using AsyncClient = AsyncClientClass;
AsyncClient async_client1(ssl_client1), async_client2(ssl_client2);

// DATABASE
RealtimeDatabase Database;
AsyncResult dbResult;

// SENSOR
DHT dht(DHTPIN, DHTTYPE);

unsigned long lastMillis = 0;

void setup() {
    Serial.begin(115200);

    pinMode(led1, OUTPUT);
    digitalWrite(led1, OFF);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(300);
    }

    Serial.println("\nConnected!");
    Serial.println(WiFi.localIP());

    // SSL
    ssl_client1.setInsecure();
    ssl_client2.setInsecure();

    ssl_client1.setConnectionTimeout(2000);
    ssl_client1.setHandshakeTimeout(10);
    ssl_client2.setConnectionTimeout(2000);
    ssl_client2.setHandshakeTimeout(10);

    // INIT FIREBASE
    initializeApp(async_client1, app, getAuth(user_auth), processData, "authTask");

    app.getApp<RealtimeDatabase>(Database);
    Database.url(DATABASE_URL);

    dht.begin();
}

void loop() {
    app.loop();
    processData(dbResult);

    if (millis() - lastMillis > 5000) {
        lastMillis = millis();

        if (app.ready()) {

            float h = dht.readHumidity();
            float t = dht.readTemperature();

            if (isnan(h) || isnan(t)) {
                Serial.println("Gagal membaca DHT!");
                return;
            }

            Serial.printf("Kirim -> Suhu: %.2f°C | Kelembaban: %.2f%%\n", t, h);

            // ✅ FIX PATH SESUAI FIREBASE
            Database.set<float>(async_client1, "/iot-sendy-databse/suhu", t, processData, "suhuTask");
            Database.set<float>(async_client1, "/iot-sendy-databse/kelembaban", h, processData, "kelembapanTask");

            // ✅ AMBIL DATA LED
            String value = Database.get<String>(async_client2, "/iot-sendy-databse/led");

            if (value == "1") {
                digitalWrite(led1, ON);
            } else {
                digitalWrite(led1, OFF);
            }
        }
    }
}

// HANDLE RESPONSE
void processData(AsyncResult &aResult)
{
    if (!aResult.isResult())
        return;

    if (aResult.isEvent())
        Firebase.printf("Event: %s | %s | code: %d\n",
            aResult.uid().c_str(),
            aResult.eventLog().message().c_str(),
            aResult.eventLog().code());

    if (aResult.isDebug())
        Firebase.printf("Debug: %s | %s\n",
            aResult.uid().c_str(),
            aResult.debug().c_str());

    if (aResult.isError())
        Firebase.printf("Error: %s | %s | code: %d\n",
            aResult.uid().c_str(),
            aResult.error().message().c_str(),
            aResult.error().code());

    if (aResult.available())
        Firebase.printf("Data: %s | %s\n",
            aResult.uid().c_str(),
            aResult.c_str());
}