#include <SoftwareSerial.h>
#include <TinyGPS.h>

#include <SPI.h>
#include <RH_RF95.h>

RH_RF95 rf95;

TinyGPS gps;
SoftwareSerial ss(3, 4); // Arduino RX, TX ,

//Functions
static void smartdelay(unsigned long ms);
static char *print_int(unsigned long val, unsigned long invalid, int len);
static char *print_date(TinyGPS &gps);
int printFloat(double f, int digits = 2);
int r = 0;

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

void(* resetFunc) (void) = 0;

void loop()
{
  char buffer[100];
  char *tmp1;
  char *tmp2;
  char *tmp3; 
  long tmp4, tmp5, tmp6;
  long lat, lon;
  unsigned long age;
  bool newdata = false;
  unsigned long start = millis();

  while (millis() - start < 1000) {
    if (ss.available()) {
      char c = ss.read();

      if (gps.encode(c)) {
        newdata = true;

      }
    }
  }
  
  memset(buffer, '\0', sizeof(buffer));
  tmp1 = print_int(gps.satellites(), TinyGPS::GPS_INVALID_SATELLITES, 5);
  tmp2 = print_int(gps.hdop(), TinyGPS::GPS_INVALID_HDOP, 5);
  tmp3 = print_date(gps);
 
  Serial.print(tmp1);
  Serial.print(" ");
  Serial.print(tmp2);
  Serial.print(" ");
  Serial.print(tmp3);
  Serial.print(" "); 
  gps.get_position(&lat, &lon, &age);   
  Serial.print(lat);
  Serial.print(" ");  
  Serial.print(lon); 
  Serial.print(" ");   
  Serial.print(age);  


  sprintf(buffer, "%s %s %s %li %li %li", tmp1, tmp2, tmp3, lat, lon, age);
  rf95.send((uint8_t*) buffer, sizeof(buffer));
  //delay(1000);
  //Serial.print(" should have sent something");
  rf95.waitPacketSent();

  Serial.println();
  smartdelay(1000);
  if(TinyGPS::GPS_INVALID_SATELLITES){
  r++;
  if(r==5){
    resetFunc(); //reset
  } else {
    //continue on
    }
  }
}

static char *print_int(unsigned long val, unsigned long invalid, int len)
{
  int i = 32;
  char *sz;
  sz = (char*) malloc (i + 1);

  if (val == invalid)
    strcpy(sz, "*******");
  else
    sprintf(sz, "%ld", val);
  sz[len] = 0;
  smartdelay(0);    
  return sz;
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

static char *print_date(TinyGPS &gps)
{
  int i = 32;
  char *sz;
  sz = (char*) malloc (i + 1);
  
  int year;
  byte month, day, hour, minute, second, hundredths;
  unsigned long age;
  gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);
  if (age == TinyGPS::GPS_INVALID_AGE) {
    strcpy(sz, "******** ****** ");
  }
  else
  {
    sprintf(sz, "%02d%02d%02d %02d%02d%02d ",
            month, day, year, hour - 4, minute, second);
  }
  print_int(age, TinyGPS::GPS_INVALID_AGE, 5);
  smartdelay(0);  
  return sz;
}

