#include "FreqCountESP.h"
//Two LEDs connected from GPIO pin, through 1K resistor, and to GND.
#define Status_LED 18 //white
#define Motion_LED 32 //green
#define diag_LED 27 //red
#define HB100_Amp 34 //amplifier output in 

void setup() {
  pinMode(Status_LED, OUTPUT);
  pinMode(Motion_LED, OUTPUT);
  pinMode(diag_LED, OUTPUT);
  digitalWrite(Status_LED, HIGH);
  double HB_In; 
  //start a frequency meaurement
  int ms = 1000;
  FreqCountESP.begin(HB100_Amp, ms);
  Serial.begin(9600);
}

void loop() {
  
  if (FreqCountESP.available()){
    uint32_t frequency = FreqCountESP.read();
   
    Serial.print("Frequency is: ");
    Serial.println(frequency);

    if (frequency > 0){
     digitalWrite(Motion_LED, HIGH);
    }
    else digitalWrite(Motion_LED, LOW);
  }

}
