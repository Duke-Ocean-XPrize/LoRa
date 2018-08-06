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
  //rf95.setModemConfig(RH_RF95::Bw125Cr45Sf128);
  //rf95.setFrequency(915.0);
  if (!rf95.init())
    Serial.println("init failed");
  /*else
    Serial.println("init successful");*/
}

void loop() {
 //Serial.println("start of loop");
 uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
 uint8_t len = sizeof(buf);
 if (rf95.recv((uint8_t*) buf, (uint8_t*)len)){
    Serial.print((char*) buf);
 } else {
    Serial.print("***** ***** ******** ******  ********* ********* **********");
 }
 Serial.println();
 delay(1000);
}
