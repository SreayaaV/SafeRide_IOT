#include <ThingSpeak.h>
#include <ESP8266WiFi.h>
#include <Wire.h>

const char* ssid = "******";                 // Replace with your Wi-Fi network name
const char* password = "******";             // Replace with your Wi-Fi password

const char* server = "api.thingspeak.com";
unsigned long channelID = "*******";         //Replace with your ThingSpeak channel ID
const char* readAPIKey = "*******";          //Replace with your ThingSpeak READ API key
unsigned int dataFieldOne = *;               //Replace with the datafield number

float gasValue = 0.0;
WiFiClient client;

const int relayPin = D3;
const int thresholdValue = 500;
const int buzzerPin = D4;

void setup() {
  Serial.begin(115200);
  Serial.println("Start");
  connectWiFi();
  gasValue = readTSData(channelID, dataFieldOne);
  delay(1000);
}

void loop() {
  delay(2000);
  Serial.println("Waiting...");

  gasValue = readTSData(channelID, dataFieldOne);
  Serial.println("Gas Sensor Value: " + String(gasValue, 2));

  if (gasValue < thresholdValue) {
    pinMode(relayPin, OUTPUT);      //Sets the relayPin as Output 
    digitalWrite(relayPin, HIGH);   // Turn on the relay
    Serial.println("Motor is ON");
  } else {
    pinMode(buzzerPin, OUTPUT);
    digitalWrite(buzzerPin, HIGH);  //Turn on the buzzer
    digitalWrite(relayPin, LOW);    // Turn off the relay
    Serial.println("Motor is OFF");
}
}
void connectWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("Connected");
  ThingSpeak.begin(client);
}

float readTSData(long TSChannel, unsigned int TSField) {
  float data = ThingSpeak.readFloatField(TSChannel, TSField, readAPIKey);
  Serial.println("Data read from ThingSpeak: " + String(data, 2));
  return data;
}
