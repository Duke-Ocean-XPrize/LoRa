#include <SPI.h>
#include <RH_RF95.h>
#include <SoftwareSerial.h>

RH_RF95 rf95;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial);
  //Serial.println("booted");
  //Serial.println("RF95 initializing");
  delay(100);
  //rf95.setFrequency(915.0);
  //rf95.setModemConfig(RH_RF95::Bw125Cr45Sf128);
  if (!rf95.init())
    Serial.println("init failed");
  /*else
    Serial.println("init successful");*/
}

void loop() {
 //Serial.println("start of loop");
 char buffer[100];
 if (rf95.recv((uint8_t*) buffer, sizeof(buffer)+1)){
    Serial.println(buffer);
 } else {
    Serial.println("***** ***** ******** ******  ********* ********* **********");
 }
 delay(1000);
}
