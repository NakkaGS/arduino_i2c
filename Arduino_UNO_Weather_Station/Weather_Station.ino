// ---------------------------------------------------------------------------
// MULTISENSORES - Estação Meteorologica - POR I2C  Gabriel Nakata - 2016
// ---------------------------------------------------------------------------

#include "DHT.h"
#include <Wire.h>

#define SONAR_NUM_1     4 // Number of Sensors
#define ARDUINO_1       15 // Arduino Address

#define DHTPIN A1 // analog pin A1 
#define DHTTYPE DHT11 

int RainIn = A0; // analog pin A0
int RainDigitalIn = 2; // digital pin 2
int RainVal;
int RainI2C;
boolean IsRaining = false;
String strRaining;

float h;
float t;

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  pinMode(RainDigitalIn,INPUT);
  Serial.println("DHT11 Test!");
  dht.begin();
  
  Wire.begin(ARDUINO_1); // start I2C with the address
  Wire.onRequest(requestEvent); // if the MASTER request data, it will do the requestEvent
}

void loop() {

  //Raining Sensor
  RainVal = analogRead(RainIn);
  RainI2C = RainVal/4; //divides by 4 bacause i2c goes until 255. The Master will multiple by 4 to get the real result
  
  IsRaining = !(digitalRead(RainDigitalIn));

  if(IsRaining){
    strRaining = "Yes";
  }
  else{
    strRaining = "No";
  }
  
  Serial.print("Raining?: ");
  Serial.print(strRaining);  
  Serial.print("\t   Water Level: ");
  Serial.print(RainVal);
  
  delay(100);
  
  //DHT11
  h = dht.readHumidity();
  t = dht.readTemperature();
  if (isnan(t) || isnan(h)) // check if it not a number
  {
    Serial.println("Failed to read DHT11");
  } 
  else
  {
    Serial.print("  ///////  Huminity: ");
    Serial.print(h);
    Serial.print(" %t");
    Serial.print("  Temperature: ");
    Serial.print(t);
    Serial.println(" *C");
  }
}

void requestEvent(){
  
  uint8_t data[SONAR_NUM_1];  // data to be sent to MASTER

    data[0] = h; //HumiHuminitydade
    data[1] = t; //Temperature
    data[2] = IsRaining;
    data[3] = RainI2C;
    
  Wire.write(data,SONAR_NUM_1); // vector, bytes numbers to send
} 

// ---------------------------------------------------------------------------
// MULTISENSORES - Estação Meteorologica - POR I2C  Gabriel Nakata - 2016
// ---------------------------------------------------------------------------

