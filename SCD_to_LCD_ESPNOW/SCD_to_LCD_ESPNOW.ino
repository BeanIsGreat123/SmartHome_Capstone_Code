#include <Arduino.h>
#include <SensirionI2cScd30.h>
#include <Wire.h>
#include <WiFi.h>
#include <esp_now.h>
SensirionI2cScd30 sensor;

static char errorMessage[128];
static int16_t error;
//define struct that will be sent
typedef struct message{
    float t;
    float h;
    float co2;
} message;
//Instantiate variable
message data;

uint8_t address[] = {0xA0, 0xB7, 0x65, 0x49, 0x1B, 0x20};

//peer info
esp_now_peer_info_t peer;

void OnSend(const uint8_t *mac_addr, esp_now_send_status_t status){
  Serial.print("Packet send status:");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery success" : "Delivery Fail");
}

void setup() {

    Serial.begin(115200);

    while (!Serial) {
        delay(100);
    }
    Wire.begin();
    sensor.begin(Wire, SCD30_I2C_ADDR_61);

    sensor.stopPeriodicMeasurement();
    sensor.softReset();
    delay(2000);
    uint8_t major = 0;
    uint8_t minor = 0;
    error = sensor.readFirmwareVersion(major, minor);
    if (error != NO_ERROR) {
        Serial.print("Error trying to execute readFirmwareVersion(): ");
        errorToString(error, errorMessage, sizeof errorMessage);
        Serial.println(errorMessage);
        return;
    }
    Serial.print("firmware version major: ");
    Serial.print(major);
    Serial.print("\t");
    Serial.print("minor: ");
    Serial.print(minor);
    Serial.println();
    error = sensor.startPeriodicMeasurement(0);
    if (error != NO_ERROR) {
        Serial.print("Error trying to execute startPeriodicMeasurement(): ");
        errorToString(error, errorMessage, sizeof errorMessage);
        Serial.println(errorMessage);
        return;
    }
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

}

void loop() {
    float co2Concentration = 0.0;
    float temperature = 0.0;
    float humidity = 0.0;
    delay(1500);
    error = sensor.blockingReadMeasurementData(co2Concentration, temperature,
                                               humidity);
    if (error != NO_ERROR) {
        Serial.print("Error trying to execute blockingReadMeasurementData(): ");
        errorToString(error, errorMessage, sizeof errorMessage);
        Serial.println(errorMessage);
        return;
    }
    data.co2 = co2Concentration;
    data.t = temperature;
    data.h = humidity;
    Serial.print("co2Concentration: ");
    Serial.print(data.co2);
    Serial.print("\t");
    Serial.print("temperature: ");
    Serial.print(data.t);
    Serial.print("\t");
    Serial.print("humidity: ");
    Serial.print(data.h);
    Serial.print("\t");
    Serial.println();
    esp_err_t result = esp_now_send(address, (uint8_t *) &data, sizeof(data));
    if(result == ESP_OK){
      Serial.print("Sending confirmed");
    } 
}
