#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "Shubham";
const char* password = "123456789";
const char* apiKey = "e1171a938db04949b4d195117240211";
const char* location = "London"; 

void connectToWiFi() {
  Serial.print("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);
  
  unsigned long startAttemptTime = millis();
  
  // Attempt to connect to Wi-Fi within 10 seconds
  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) {
    delay(500);
    Serial.print(".");
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnected to Wi-Fi");
  } else {
    Serial.println("\nFailed to connect to Wi-Fi. Rebooting...");
    ESP.restart();
  }
}

void setup() {
  Serial.begin(115200);
  connectToWiFi();
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "http://api.weatherapi.com/v1/current.json?key=" + String(apiKey) + "&q=" + String(location);

    Serial.println("Requesting data from: " + url);
    http.begin(url.c_str());
    
    int httpResponseCode = http.GET();
    
    if (httpResponseCode == 200) {
      String payload = http.getString();
      Serial.println("Weather Data:");
      Serial.println(payload);
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
      
      if (httpResponseCode == -1) {
        Serial.println("Check your network connection or API URL.");
      } else if (httpResponseCode == 403) {
        Serial.println("Invalid API key or access denied.");
      } else if (httpResponseCode == 404) {
        Serial.println("Requested data not found. Check location or API endpoint.");
      }
    }
    
    http.end();
  } else {
    Serial.println("Wi-Fi Disconnected. Reconnecting...");
    connectToWiFi();
  }
  
  delay(60000);  // Wait 1 minute before the next request
}
