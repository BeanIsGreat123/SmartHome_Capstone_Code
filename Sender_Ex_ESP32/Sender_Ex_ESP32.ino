#include <Arduino.h>
#include <SensirionI2cScd30.h>
#include <Wire.h>
#include <WiFi.h>
#include <esp_now.h>
#define PIR_PIN 13
#define OUTPIN  32
SensirionI2cScd30 sensor;

static char errorMessage[128];
static int16_t error;
//define struct that will be sent
typedef struct message{
    int id = 2;
    float t = 8008135;
    float h = 1000;
    float co2 = 43110;
    bool pir = 1;
} message;
//Instantiate variable
message data;

uint8_t address[] = {0xB0, 0xB2, 0x1C, 0xA8, 0x99, 0xAC};

//peer info
esp_now_peer_info_t peer;

void OnSend(const uint8_t *mac_addr, esp_now_send_status_t status){
  Serial.print("Packet send status:");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery success" : "Delivery Fail");
}

void setup() {
    pinMode(PIR_PIN, INPUT);
    pinMode(OUTPIN, OUTPUT);
    digitalWrite(OUTPIN, HIGH);
//configure Wifi Mode
    WiFi.mode(WIFI_STA);
//initialize esp now
if(esp_now_init() != ESP_OK){
  Serial.println("error starting esp now");
}
//Register the send callback
esp_now_register_send_cb(OnSend);
memcpy(peer.peer_addr, address, 6);
peer.channel = 0;
peer.encrypt = false;
//connect and verify connection
if(esp_now_add_peer(&peer) != ESP_OK){
  Serial.println("Failed to add peer");
  return;
}
    Serial.begin(115200);
}

void loop() {
    bool pir = digitalRead(PIR_PIN);
    float co2Concentration = (float) random(4000);
    float temperature = (float) random(40);
    float humidity = (float) random(50);
    delay(3000);
    data.co2 = co2Concentration;
    data.t = temperature;
    data.h = humidity;
    data.pir = pir;
    Serial.print("co2Concentration: ");
    Serial.print(data.co2);
    Serial.print("\t");
    Serial.print("temperature: ");
    Serial.print(data.t);
    Serial.print("\t");
    Serial.print("humidity: ");
    Serial.print(data.h);
    Serial.print("\t");
    Serial.print("PIR: ");
    Serial.print(pir);
    Serial.println();
    esp_err_t result = esp_now_send(address, (uint8_t *) &data, sizeof(data));
    if(result == ESP_OK){
      Serial.print("Sending confirmed");
    }
}
