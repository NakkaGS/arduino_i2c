// ---------------------------------------------------------------------------
// MULTISENSORES - MASTER + LCD - BY I2C  Gabriel Nakata - 2016
// ---------------------------------------------------------------------------

#include <Wire.h> // I2C Library.
#include <LiquidCrystal.h> //LCD Library.

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

//////// LCD ////////
int fadeValue = 200;
int lcd_key     = 0;
int adc_key_in  = 0;

#define btnRIGHT 0 //Define LCD Button Right
#define btnUP 1 //Define LCD Button Up
#define btnDOWN 2 //Define LCD Button Down
#define btnLEFT 3 //Define LCD Button Left
#define btnSELECT 4 //Define LCD Button Select
#define btnNONE 5 //Define None

//////// LCD ////////

#define SENSOR_NUM_1 4 // Number of Sensors on Arduino 1 (UNO)
#define ARDUINO_1 15 // Arduino 1 Address (NANO)
#define SENSOR_NUM_2 // Number of Sensors on Arduino 2 (NANO)
#define ARDUINO_2 8 // Arduino 2 Address (NANO)

int arduino_1[SONAR_NUM_1]; //Sensor Data from Arduino 1 (UNO)
int arduino_2[SONAR_NUM_2]; //Sensor Data from Arduino 2 (NANO)

int RainVal;
int IsRaining;
int Temp;
int Hum;
String strRaining;
int v = 0;

int read_LCD_buttons(){               // read botton
    adc_key_in = analogRead(0);        

    if (adc_key_in > 1000) return btnNONE; 

     if (adc_key_in < 50)   return btnRIGHT;  
     if (adc_key_in < 195)  return btnUP; 
     if (adc_key_in < 380)  return btnDOWN; 
     if (adc_key_in < 555)  return btnLEFT; 
     if (adc_key_in < 790)  return btnSELECT;  

    return btnNONE;                // If nothing was pressed it will return NONE
}

void setup() {

  Wire.begin(); // add the master in the I2C
  Serial.begin(115200); 
  lcd.begin(16, 2);               // start the library
  lcd.setCursor(0,0);             // set the LCD cursor position 
  lcd.print("Push the buttons"); 
}

void loop() {
  ////////// Receive Data from SLAVE 1 - Arduino 1 (UNO) ////////// 
  
  Wire.requestFrom(ARDUINO_1, SONAR_NUM_1);    // request data from SLAVE 1
  
  Serial.println();
  Serial.println("/////////////// Data from Arduino 1 ///////////////");
  
  int i = 0;
  while (Wire.available()) { 
    for(i = 0; i< SONAR_NUM_1; i++){
      int c = Wire.read(); // receive the byte as integer
      arduino_1[i] = c;
    }
    Temp = arduino_1[0];
    Hum = arduino_1[1];
    IsRaining = arduino_1[2];
    RainVal = arduino_1[3]*4;
    Serial.print("Temp: ");
    Serial.print(Temp);
    Serial.print("        Huminity: ");
    Serial.print(Hum);
    Serial.print("        Raining:  ");
    if(IsRaining){
    strRaining = "Y";
    }
    else{
      strRaining = "N";
    }
    Serial.print(strRaining);
    Serial.print("        Water Level: ");
    Serial.println(RainVal);
  }

  ////////// Receive Data from SLAVE 2 - Arduino 2 (NANO) ////////// 
  
  Wire.requestFrom(ARDUINO_2, SONAR_NUM_2);    // request data from SLAVE 2
  
  Serial.println();
  Serial.println("/////////////// Data from Arduino 2 ///////////////");
  
  while (Wire.available()) { 
    for(i = 0; i< SONAR_NUM_2; i++){
      int c = Wire.read(); // receive the byte as integer
      arduino_2[i] = c;
      Serial.print("Distance Sensor ");
      Serial.print(i);
      Serial.print(": ");
      Serial.println(arduino_2[i]);
    }
  }

  delay(100);

  analogWrite (10, fadeValue);    

  lcd_key = read_LCD_buttons();
  switch (lcd_key){
        case btnRIGHT:{             //  Press left or right to change the Data
            if(v == 0) {
              lcd.setCursor(0,0);
              lcd.print("Arduino 2");
            }
            if(v == 1) {
              lcd.setCursor(0,0);
              lcd.print("Arduino 1");
            }
            delay(500);
            lcd.clear();
            v++;
            break;
       }
       case btnLEFT:{
            if(v == 0) {
              lcd.setCursor(0,0);
              lcd.print("Arduino 2");
            }
            if(v == 1) {
              lcd.setCursor(0,0);
              lcd.print("Arduino 1");
            }
            delay(500);
            lcd.clear();
            v++;
            break;
       }   
  }

  if(v > 1){ //se v for maior que 1, v sera = 0
    v = 0;
  }
  
  //Serial Monitor - Arduino UNO - Weather Station
  if(v == 0){ 
    lcd.setCursor(0,0);
    lcd.print("Temp: ");
    lcd.setCursor(6,0);
    lcd.print(Temp);
    lcd.setCursor(9,0);
    lcd.print("Hum: ");
    lcd.setCursor(14,0);
    lcd.print(Hum);

    lcd.setCursor(0,1);
    lcd.print("Raining ");
    lcd.setCursor(6,1);
    lcd.print(strRaining);
    lcd.setCursor(8,1);
    lcd.print("LVL ");
    lcd.setCursor(12,1);
    lcd.print(RainVal);
    if(RainVal < 1000){
      lcd.setCursor(15,1);
      lcd.print(" ");
    }
  }

  //Serial Monitor - Arduino NANO - Distance Sensors
  if(v == 1){ 
    lcd.setCursor(0,0);
    lcd.print("Distance 1: ");
    lcd.setCursor(13,0); 
    lcd.print(arduino_2[0]);
    if(arduino_2[0]<10){
      lcd.setCursor(14,0); 
      lcd.print("  ");
    }if(arduino_2[0]>10 && arduino_2[0]<100){
      lcd.setCursor(15,0); 
      lcd.print(" ");
    }
    
    lcd.setCursor(0,1);
    lcd.print("Distance 2: ");
    lcd.setCursor(13,1); 
    lcd.print(arduino_2[1]);
    if(arduino_2[1]<10){
      lcd.setCursor(14,1); 
      lcd.print("  ");
    }if(arduino_2[1]>10 && arduino_2[1]<100){
      lcd.setCursor(15,1); 
      lcd.print(" ");
    }
  }
}

// ---------------------------------------------------------------------------
// MULTISENSORS - MASTER + LCD - BY I2C  Gabriel Nakata - 2016
// ---------------------------------------------------------------------------
