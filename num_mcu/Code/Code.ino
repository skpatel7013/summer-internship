#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>

const char* ssid = "Shubham";
const char* password = "shubh1138";

ESP8266WebServer server;
String receivedData = "";

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");

  server.on("/data", handleData);  // Route to handle incoming data
  server.begin();
}

void loop() {
  server.handleClient();  // Handle incoming client requests
  sendDataToServer();
}

void handleData() {
  if (server.hasArg("data")) {
    receivedData = server.arg("data");
    Serial.println("Received data from STM32: " + receivedData);
  }

  server.send(200, "text/plain", "Data received");  // Send response to STM32
}

void sendDataToServer() {
  if (!receivedData.isEmpty()) {
    WiFiClient client;
    HTTPClient http;

    // Modify the URL to match your server or web page
    const char* serverUrl = "http://your_server_endpoint";

    if (http.begin(client, serverUrl)) {
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      int httpResponseCode = http.POST("data=" + receivedData);

      if (httpResponseCode > 0) {
        Serial.println("Data sent successfully to web page");
      } else {
        Serial.println("Error sending data to web page");
      }

      http.end();
    }

    receivedData = "";  // Clear the received data
  }
}
