//function for the output of the ESP32
#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#define LED_PIN 12
#define ALED_PIN 25
int ledchannel = 0;

typedef struct message {
  int button;
  int pot;
} message;

message data;

void OnInput(const uint8_t * info, const uint8_t *incomingData, int length){
  memcpy(&data, incomingData, length);
digitalWrite(LED_PIN, data.button);
ledcWrite(ledchannel, data.pot);
}

void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  pinMode(LED_PIN, OUTPUT);
  ledcSetup(ledchannel, 5000, 8);
  ledcAttachPin(ALED_PIN, ledchannel);
  if(esp_now_init() != ESP_OK){
  Serial.println("error starting esp now");
  }
  esp_now_register_recv_cb(OnInput);
}

void loop() {
  
}