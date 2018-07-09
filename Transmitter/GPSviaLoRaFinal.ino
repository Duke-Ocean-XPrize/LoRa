#include <SoftwareSerial.h>
#include <TinyGPS.h>

#include <SPI.h>
#include <RH_RF95.h>

RH_RF95 rf95;

TinyGPS gps;
SoftwareSerial ss(3, 4); // Arduino RX, TX ,

//Functions
static void smartdelay(unsigned long ms);
//int r = 0;
long inc=0;

void setup()
{
  // initialize both serial ports:
  Serial.begin(9600);  // Serial to print out GPS info in Arduino IDE
  ss.begin(9600); // SoftSerial port to get GPS data.
  //rf95.setFrequency(915);
  while (!Serial);
  if (!rf95.init())
    Serial.print("init failed");

}

//void(* resetFunc) (void) = 0;

void loop()
{
  bool newdata = false;
  char buffer[100];
  long lat, lon;
  unsigned long age;
  
  unsigned long start = millis();

  while (millis() - start < 1000) {
    if (ss.available()) {
      char c = ss.read();

      if (gps.encode(c)) {
        newdata = true;

      }
    }
  }
 
  gps.get_position(&lat, &lon, &age);   
  char* timestamp = format_timestamp(gps);
  sprintf(buffer, "%u %lu %s %li %li %li %d", gps.satellites(), gps.hdop(), timestamp, lat, lon, age, inc);
  inc++;
  //strcpy(buffer,"POTATO");
  Serial.println(buffer);
  rf95.send((uint8_t*) buffer, strlen(buffer));
 
  rf95.waitPacketSent();

  Serial.println();
  //wait a short while for the gps to encode its new coordinates
  smartdelay(100);
  //the code stops after 5 transmissions if satellites are not detected, so I added this auto reset function so I don't have to manually reset the transmitter
  /*if(TinyGPS::GPS_INVALID_SATELLITES){
    r++;
    if(r==5){
      resetFunc(); //reset
    } else {
      //continue on
    }
  }*/
}

static void smartdelay(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    while (ss.available())
    {
      gps.encode(ss.read());
    }
  } while (millis() - start < ms);
}

static char* format_timestamp(TinyGPS &gps)
{
  static char buf[32];
  
  int year;
  byte month, day, hour, minute, second, hundredths;
  unsigned long age;
  gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);
  sprintf(buf, "%02d-%02d-%02d_%02d:%02d:%02d", month, day, year, hour, minute, second);
  smartdelay(0);  
  return buf;
}

