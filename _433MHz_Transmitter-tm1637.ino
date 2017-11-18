#include <VirtualWire.h>
#include "DHT.h"
#include "TM1637.h"
#define DHTPIN 2  

#define DHTTYPE DHT22 
#define CLK 3//Pins for TM1637       
#define DIO 4
TM1637 tm1637(CLK,DIO);
const int led_pin = 13;
const int transmit_pin = 12;

struct package
{
  float temperature ;
  float humidity ;
};


typedef struct package Package;
Package data;

DHT dht(DHTPIN, DHTTYPE);

void setup()
{
    // Initialise the IO and ISR
    vw_set_tx_pin(transmit_pin);
    vw_set_ptt_inverted(true); // Required for DR3100
    vw_setup(2000);       // Bits per sec
    pinMode(led_pin, OUTPUT);
     tm1637.init();
  tm1637.set(BRIGHT_TYPICAL); 
}



void loop()
{
  digitalWrite(led_pin, HIGH); // Flash a light to show transmitting
  readSensor();
  vw_send((uint8_t *)&data, sizeof(data));
  vw_wait_tx(); // Wait until the whole message is gone
  digitalWrite(led_pin, LOW);
  delay(2000);
}

void readSensor()
{
 dht.begin();
 delay(1000);
 data.humidity = dht.readHumidity();
 data.temperature = dht.readTemperature();
  int humidity = dht.readHumidity();
   int temp = dht.readTemperature();  
   int digitoneT = temp / 10;
 int digittwoT = temp % 10;
 
 int digitoneH = humidity / 10;
 int digittwoH = humidity % 10;
  
 
    tm1637.display(0,digitoneT); 
    tm1637.display(1,digittwoT);
    tm1637.display(2,18); // put degree
    tm1637.display(3,12);  // put a C at the end
    
    delay (3000);
    
    tm1637.display(0,17);  // empty
    tm1637.display(1,17);  // empty
    tm1637.display(2,17);  // empty
    tm1637.display(3,17);  // empty


    tm1637.display(0,digitoneH); 
    tm1637.display(1,digittwoH);
    tm1637.display(2,19);  // r
    tm1637.display(3,20);  // h
 
  
  //Fastest should be once every two seconds.
    delay(3000);
}

