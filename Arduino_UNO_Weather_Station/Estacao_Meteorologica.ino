// ---------------------------------------------------------------------------
// MULTISENSORES - Estação Meteorologica - POR I2C  Gabriel Nakata - 2016
// ---------------------------------------------------------------------------

#include "DHT.h"
#include <Wire.h>

#define SONAR_NUM_1     4 // Numero de Sensores do Arduino 1.
#define ARDUINO_1       15 //Endereco do Arduino 1

#define DHTPIN A1 //pino analógico A1 
#define DHTTYPE DHT11 

int RainIn = A0; //pino analógico A0
int RainDigitalIn = 2; //pino digital 2
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
  Serial.println("Teste do DHT11!");
  dht.begin();
  
  Wire.begin(ARDUINO_1);                // inicia o i2c com o endereço de ARDUINO_1
  Wire.onRequest(requestEvent); // caso seja solicitado este slave, ele vai para a funcao que foi atribuido
}

void loop() {

  //Sensor de Chuva
  RainVal = analogRead(RainIn);
  RainI2C = RainVal/4; //divide por 4 pois no i2c vai até 255, desta forma divide por 4 no slave e multiplica por 4 no master
  
  IsRaining = !(digitalRead(RainDigitalIn));

  if(IsRaining){
    strRaining = "Sim";
  }
  else{
    strRaining = "Nao";
  }
  
  Serial.print("Chovendo?: ");
  Serial.print(strRaining);  
  Serial.print("\t   Level de Agua: ");
  Serial.print(RainVal);
  
  
  delay(100);
  
  //DHT11
  h = dht.readHumidity();
  t = dht.readTemperature();
  if (isnan(t) || isnan(h)) // testa se é não um numero
  {
    Serial.println("Falha na Leitura do DHT");
  } 
  else
  {
    Serial.print("  ///////  Umidade: ");
    Serial.print(h);
    Serial.print(" %t");
    Serial.print("  Temperatura: ");
    Serial.print(t);
    Serial.println(" *C");
  }
}

void requestEvent(){
  
  uint8_t data[SONAR_NUM_1];  //vetor para envio de dados para o MASTER

    data[0] = h; //Humidade
    data[1] = t; //Temperatura
    data[2] = IsRaining;
    data[3] = RainI2C;
    
  Wire.write(data,SONAR_NUM_1); // vetor, numero de bytes para enviar
} 

// ---------------------------------------------------------------------------
// MULTISENSORES - Estação Meteorologica - POR I2C  Gabriel Nakata - 2016
// ---------------------------------------------------------------------------

