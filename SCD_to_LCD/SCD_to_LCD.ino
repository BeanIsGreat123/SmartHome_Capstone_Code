/*  
 Test the tft.print() viz embedded tft.write() function

 This sketch used font 2, 4, 7

 Make sure all the display driver and pin connections are correct by
 editing the User_Setup.h file in the TFT_eSPI library folder.

 #########################################################################
 ###### DON'T FORGET TO UPDATE THE User_Setup.h FILE IN THE LIBRARY ######
 #########################################################################
 */


#include <TFT_eSPI.h> // Graphics and font library for ILI9341 driver chip
#include <SPI.h>
#include <Arduino.h>
#include <SensirionI2cScd30.h>
#include <Wire.h>
SensirionI2cScd30 sensor;

static char errorMessage[128];
static int16_t error;
int i;

#define TFT_GREY 0x5AEB // New colour

TFT_eSPI tft = TFT_eSPI();  // Invoke library


void setup(void) {
  Serial.begin(115200);
  tft.init();
  tft.setRotation(3);
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
    error = sensor.startPeriodicMeasurement(0);
    if (error != NO_ERROR) {
        Serial.print("Error trying to execute startPeriodicMeasurement(): ");
        errorToString(error, errorMessage, sizeof errorMessage);
        Serial.println(errorMessage);
        return;
    }
}

void loop() {
  i++;
  //COMMENTED OUT TO TEST, REPLACE LATER 
  /*float co2Concentration = 0.0;
  float temperature = 0.0;
  float humidity = 0.0;
  delay(150);
  error = sensor.blockingReadMeasurementData(co2Concentration, temperature, humidity);
  if (error != NO_ERROR) {
        Serial.print("Error trying to execute blockingReadMeasurementData(): ");
        errorToString(error, errorMessage, sizeof errorMessage);
        Serial.println(errorMessage);
        return;
    }
  Serial.print(temperature);*/
    //ABOVE COMMENTED OUT TO TEST, REPLACE LATER 
  // Fill screen with grey so we can see the effect of printing with and without 
  // a background colour defined
  tft.fillScreen(TFT_GREEN);
  //tft.drawRect(5, 5, 310, 230, TFT_GREEN);
  tft.fillRect(5, 5, 310, 230, TFT_WHITE);
  // Set "cursor" at top left corner of display (0,0) and select font 2
  // (cursor will move to next line automatically during printing with 'tft.println'
  //  or stay on the line is there is room for the text with tft.print)
  tft.setCursor(10, 10, 4);
  // Set the font colour to be yellow with no background, set to font 7
  tft.setTextColor(TFT_GREEN);

  //ROOM SPECIFIC DISPLAY
  tft.print("KEC128");
  //END ROOM SPECIFIC DISPLAY

  //ADJUST TEXT FONT AND COLOR
  tft.setTextColor(TFT_BLACK); tft.setTextFont(2);

  //DISPLAY SENSOR VALUES 
  /*tft.print("CO2: "); tft.println(co2Concentration);
  tft.print("Temp C: "); tft.println(temperature);
  tft.print("Humidity: "); tft.println(humidity);*/
  //END DISPLAY SENSOR VALUES

  //TEST PRINT FOR MISSING SENSOR
  tft.print("Testing Testing: ");
  tft.print(i);

  //DIPLAY YCP IN CORNER
  tft.setTextColor(TFT_GREEN);
  tft.setCursor(250, 200, 4); 
  tft.print("YCP");
  delay(1000);
}