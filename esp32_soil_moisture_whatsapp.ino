/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-send-messages-whatsapp/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
*/

#include <WiFi.h>
#include <HTTPClient.h>
#include <UrlEncode.h>

const char* ssid = "airliveASUS";
const char* password = "adeairliveASUS2016";

int moistureSensorPin = 14;  // Pin where D0 is connected
int moistureState = 0;

// Phone number and API key for CallMeBot
String phoneNumber = "+40749106536";
String apiKey = "8456301";

// Timing variables
const unsigned long messageIntervalMillis = 1800000;  // 30 minutes for sending messages
#define uS_TO_S_FACTOR 1000000      /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  1800         /* Time ESP32 will sleep (in seconds) = 30 minutes */

void sendMessage(String message) {
  // Data to send with HTTP POST
  String url = "https://api.callmebot.com/whatsapp.php?phone=" + phoneNumber + "&apikey=" + apiKey + "&text=" + urlEncode(message);    
  HTTPClient http;
  http.begin(url);

  // Specify content-type header
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  // Send HTTP POST request
  int httpResponseCode = http.POST(url);
  if (httpResponseCode == 200) {
    Serial.println("Message sent successfully");
  } else {
    Serial.println("Error sending the message");
    Serial.print("HTTP response code: ");
    Serial.println(httpResponseCode);
  }

  // Free resources
  http.end();
}

void setup() {
  pinMode(moistureSensorPin, INPUT);
  
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  // Read moisture sensor data
  moistureState = digitalRead(moistureSensorPin);  // Read the digital output from Pin 13

  // Check if soil is dry
  if (moistureState == HIGH) {
    Serial.println("Soil is dry");

    // Send message if soil is dry
    sendMessage("The soil is dry, it's time to water the flowers!");
  } else {
    Serial.println("Soil is wet");
    // Send message if soil is dry
    sendMessage("The soil is wet, the flowers are very happy!");
  }

  // Print a message indicating that the ESP32 will enter deep sleep
  Serial.println("Going to sleep for 30 minutes...");

  // Configure deep sleep to wake up after 30 minutes
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  esp_deep_sleep_start();  // Enter deep sleep mode
}

void loop() {
  // No need to use the loop since ESP32 will sleep after setup() runs
}
