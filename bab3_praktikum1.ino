#include <WiFi.h>

const char* ssid = "Asalole 4G";
const char* pass = "21032006";

// Update these with values suitable for your network.
// IPAddress ip(192, 168, 43, 200); // Node static IP
// IPAddress gateway(192, 168, 43, 1);
// IPAddress subnet(255, 255, 255, 0);

// Deklarasi variabel yang sebelumnya tidak ada di kode sumber
WiFiServer server(80);
const int ledPin = 12; // Sesuaikan dengan pin LED yang digunakan

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT); 

  WiFi.begin(ssid, pass);
  // WiFi.config(ip, gateway, subnet);

  // Wifi connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.print("WiFi connected, using IP address: ");
  Serial.println(WiFi.localIP());

  server.begin(); // Memulai web server
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  Serial.println("new client");
  while (!client.available()) {
    delay(1);
  }

  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  // Match the request
  int value = LOW;
  if (request.indexOf("/LED=ON") != -1) {
    digitalWrite(ledPin, HIGH);
    value = HIGH;
  }
  if (request.indexOf("/LED=OFF") != -1) {
    digitalWrite(ledPin, LOW);
    value = LOW;
  }

  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); // do not forget this one
  
  // HTML Layout
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.print("Led pin is now: ");

  if (value == HIGH) {
    client.print("On");
  } else {
    client.print("Off");
  }

  client.println("<br><br>");
  client.println("<a href=\"/LED=ON\">Turn On</a><br>");
  client.println("<a href=\"/LED=OFF\">Turn Off</a><br>");
  client.println("</html>");
  client.println("");

  delay(1);
  Serial.println("Client disconnected");
  Serial.println("");
}