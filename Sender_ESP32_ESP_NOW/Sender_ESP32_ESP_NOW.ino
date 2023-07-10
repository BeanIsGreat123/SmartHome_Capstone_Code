#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#define LED_PIN 12
#define BUT_PIN 13
#define POT_PIN 34
#define ALED_PIN 25
int buttread = 0;
int ledchannel = 0;
int count = 0;
int storage[20];
int mean = 0;

//address of recieving esp (check before using)
uint8_t address[] = {0xB0, 0xB2, 0x1C, 0xA8, 0x99, 0xA0};

typedef struct message{
  int button;
  int pot;
} message;
//INSTANTIATE MESSAGE
message data; 

//peer info
esp_now_peer_info_t peer;

void OnSend(const uint8_t *mac_addr, esp_now_send_status_t status){
  Serial.print("Packet send status:");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery success" : "Delivery Fail");
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUT_PIN, INPUT);
  pinMode(POT_PIN, INPUT);
  Serial.begin(9600);
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
  if(esp_now_add_peer(&peer) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  // Read button and pot
buttread = digitalRead(BUT_PIN);
storage[count] = analogRead(POT_PIN);

//Monitor the pot readings
//Serial.print(storage[count]);
//Serial.print("\n");

if(count=20){
  for(int i=0; i<20; i++){
    mean = mean + storage[i];
  }
  mean=mean/20;
count = 0;
}
else{
  count++;
}
data.pot = mean;
data.button = buttread;
esp_err_t result = esp_now_send(address, (uint8_t *) &data, sizeof(data));
if(result == ESP_OK){
  Serial.print("Sending confirmed");
}
else{
  //Serial.print("Problem with last step");
}
delay(100);
}

