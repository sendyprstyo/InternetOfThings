#include <WiFi.h>

const char* ssid = "Asalole 4G";
const char* password = "21032006";

WiFiServer server(80);

int led1 = 12;
int led2 = 13;
int led3 = 14;
int led4 = 27;

bool state1 = false;
bool state2 = false;
bool state3 = false;
bool state4 = false;

void setup() {

  Serial.begin(115200);

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);

  WiFi.begin(ssid, password);

  Serial.println("Connecting to WiFi...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi Connected");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {

  WiFiClient client = server.available();

  if (client) {

    String request = client.readStringUntil('\r');
    client.flush();

    // LED1
    if (request.indexOf("/LED1ON") != -1) {
      digitalWrite(led1, HIGH);
      state1 = true;
    }
    if (request.indexOf("/LED1OFF") != -1) {
      digitalWrite(led1, LOW);
      state1 = false;
    }

    // LED2
    if (request.indexOf("/LED2ON") != -1) {
      digitalWrite(led2, HIGH);
      state2 = true;
    }
    if (request.indexOf("/LED2OFF") != -1) {
      digitalWrite(led2, LOW);
      state2 = false;
    }

    // LED3
    if (request.indexOf("/LED3ON") != -1) {
      digitalWrite(led3, HIGH);
      state3 = true;
    }
    if (request.indexOf("/LED3OFF") != -1) {
      digitalWrite(led3, LOW);
      state3 = false;
    }

    // LED4
    if (request.indexOf("/LED4ON") != -1) {
      digitalWrite(led4, HIGH);
      state4 = true;
    }
    if (request.indexOf("/LED4OFF") != -1) {
      digitalWrite(led4, LOW);
      state4 = false;
    }

    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("");

    client.println("<!DOCTYPE html>");
    client.println("<html>");
    client.println("<head>");

    client.println("<meta name='viewport' content='width=device-width, initial-scale=1'>");

    client.println("<style>");

    client.println("body{font-family:Arial;background:#0f172a;color:white;text-align:center;margin:0;}");

    client.println("h1{padding:20px;}");

    client.println(".container{display:flex;flex-wrap:wrap;justify-content:center;}");

    client.println(".card{background:#1e293b;width:200px;margin:15px;padding:20px;border-radius:15px;box-shadow:0 4px 15px rgba(0,0,0,0.4);}");


    client.println(".statusON{color:#22c55e;font-weight:bold;}");
    client.println(".statusOFF{color:#ef4444;font-weight:bold;}");

    client.println("button{width:100%;padding:12px;font-size:16px;border:none;border-radius:8px;margin-top:10px;cursor:pointer;}");

    client.println(".on{background:#22c55e;color:white;}");
    client.println(".off{background:#ef4444;color:white;}");

    client.println("</style>");

    client.println("</head>");
    client.println("<body>");

    client.println("<h1>ESP32 IoT Dashboard</h1>");

    client.println("<div class='container'>");

    // LED1
    client.println("<div class='card'>");
    client.println("<h3>LED 1</h3>");
    client.println(state1 ? "<p class='statusON'>ON</p>" : "<p class='statusOFF'>OFF</p>");
    client.println("<a href='/LED1ON'><button class='on'>ON</button></a>");
    client.println("<a href='/LED1OFF'><button class='off'>OFF</button></a>");
    client.println("</div>");

    // LED2
    client.println("<div class='card'>");
    client.println("<h3>LED 2</h3>");
    client.println(state2 ? "<p class='statusON'>ON</p>" : "<p class='statusOFF'>OFF</p>");
    client.println("<a href='/LED2ON'><button class='on'>ON</button></a>");
    client.println("<a href='/LED2OFF'><button class='off'>OFF</button></a>");
    client.println("</div>");

    // LED3
    client.println("<div class='card'>");
    client.println("<h3>LED 3</h3>");
    client.println(state3 ? "<p class='statusON'>ON</p>" : "<p class='statusOFF'>OFF</p>");
    client.println("<a href='/LED3ON'><button class='on'>ON</button></a>");
    client.println("<a href='/LED3OFF'><button class='off'>OFF</button></a>");
    client.println("</div>");

    // LED4
    client.println("<div class='card'>");
    client.println("<h3>LED 4</h3>");
    client.println(state4 ? "<p class='statusON'>ON</p>" : "<p class='statusOFF'>OFF</p>");
    client.println("<a href='/LED4ON'><button class='on'>ON</button></a>");
    client.println("<a href='/LED4OFF'><button class='off'>OFF</button></a>");
    client.println("</div>");

    client.println("</div>");
    client.println("</body>");
    client.println("</html>");

    delay(1);
  }
}