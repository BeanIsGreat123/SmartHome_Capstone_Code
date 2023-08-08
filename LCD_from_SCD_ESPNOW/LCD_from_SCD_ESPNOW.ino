#include <esp_now.h>
#include <WiFi.h>
#include <TFT_eSPI.h> // Graphics and font library for ILI9341 driver chip
#include <SPI.h>
#include <Arduino.h>
#include <SensirionI2cScd30.h>
#include <Wire.h>

//VARIABLE FOR INCREMENTING TEST VALUE, NOT NECESSARY
int i;

#define TFT_GREY 0x5AEB // New colour

TFT_eSPI tft = TFT_eSPI();  // Invoke library


static char errorMessage[128];
static int16_t error;
// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
    float t;
    float h;
    float co2;
}
struct_message;

uint8_t LCDAddress[] = {0xA0, 0xB7, 0x65, 0x49, 0x1B, 0x20};

// Create a struct_message called myData
struct_message myData;


// Create an array with all the structures

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, len);
  // Update the structures with the new incoming data
  Serial.println();
  //LCD STUFF BELOW
  tft.fillScreen(TFT_GREEN);
  //tft.drawRect(5, 5, 310, 230, TFT_GREEN);
  tft.fillRect(5, 5, 310, 230, TFT_WHITE);
  // Set "cursor" at top left corner of display (0,0) and select font 2
  // (cursor will move to next line automatically during printing with 'tft.println'
  //  or stay on the line is there is room for the text with tft.print)
  tft.setCursor(20, 20, 4);
  // Set the font colour to be yellow with no background, set to font 7
  tft.setTextColor(TFT_GREEN);

  //ROOM SPECIFIC DISPLAY
  tft.println("KEC128");
  //END ROOM SPECIFIC DISPLAY

  //ADJUST TEXT FONT AND COLOR
  tft.setCursor(20, 50, 4);
  tft.setTextColor(TFT_BLACK);

  //DISPLAY SENSOR VALUES 
  tft.print("CO2: "); tft.println(myData.co2);
  tft.setCursor(20, tft.getCursorY(), 4);
  tft.print("Temp C: "); tft.println(myData.t);
  tft.setCursor(20, tft.getCursorY(), 4);
  tft.print("Humidity: "); tft.println(myData.h);
  //END DISPLAY SENSOR VALUES

  //TEST PRINT FOR MISSING SENSOR
  //tft.print("Testing Testing: ");
  //tft.print(i);

  //DIPLAY YCP IN CORNER
  tft.setTextColor(TFT_GREEN);
  tft.setCursor(250, 200, 4); 
  tft.print("YCP");
  //LCD STUFF ABOVE
}
 
void setup() {
  //Initialize Serial Monitor
  Serial.begin(115200);
  
  tft.init();
  tft.setRotation(3);
  while (!Serial) {
        delay(100);
    }
  //Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  //Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {

}