//#include <SoftwareSerial.h>
#include <Wire.h>
#include "Adafruit_ADS1015.h"


Adafruit_ADS1115 ads;


int tmpAddress = B1001001;
int ResolutionBits = 10;

int i = 0;
int temp = 10;
int humidity = 900;
int pressure = 123;
int global = 780;
int direct = 600;
char buffer[255];

int adc0, adc1, adc2, adc3;

float celsius;
char str_temp[6];

float getTemperature(){
  Wire.requestFrom(tmpAddress,2);
  byte MSB = Wire.read();
  byte LSB = Wire.read();
 
  int TemperatureSum = ((MSB << 8) | LSB) >> 4;
  celsius = TemperatureSum*0.0625;

}

void SetResolution(){
  if (ResolutionBits < 9 || ResolutionBits > 12) exit;
  Wire.beginTransmission(tmpAddress);
  Wire.write(B00000001); //addresses the configuration register
  Wire.write((ResolutionBits-9) << 5); //writes the resolution bits
  Wire.endTransmission();
 
  Wire.beginTransmission(tmpAddress); //resets to reading the temperature
  Wire.write((byte)0x00);
  Wire.endTransmission();
}


void setup() {
  // put your setup code here, to run once:
 //set up ads
 Serial.begin(9600);
 Serial.println("Startup");
  Serial.println("Datagram is celsius in degrees, humidity adc val,pressure adc,global adc,direct adc.");
 ads.setGain(GAIN_TWOTHIRDS);
 ads.begin();
 
 Wire.begin();  
 SetResolution();
}



void loop() {
  getTemperature();
  dtostrf(celsius, 4, 2, str_temp);
  delay(2000);
  
  //fetch values from ads1115
  humidity = ads.readADC_SingleEnded(0);
  delay(100);
  pressure = ads.readADC_SingleEnded(1);
  delay(100);
  global = ads.readADC_SingleEnded(2);
  //fetch tmp100
  
  //send to base
  sprintf(buffer,"D:%s,%d,%d,%d",str_temp,humidity,pressure,global,direct);
  Serial.println(buffer);
}
