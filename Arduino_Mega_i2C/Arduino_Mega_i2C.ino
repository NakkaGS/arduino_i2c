// ---------------------------------------------------------------------------
// MULTISENSORES - MASTER + LCD - POR I2C  Gabriel Nakata - 2016
// ---------------------------------------------------------------------------

#include <Wire.h> // Biblioteca do I2C
#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

//////// LCD ////////
int fadeValue = 200;
int lcd_key     = 0;
int adc_key_in  = 0;

#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5
//////// LCD ////////

#define SONAR_NUM_1     4 // Numero de Sensores do Arduino 1.
#define ARDUINO_1       15 //Endereco do Arduino 1

#define SONAR_NUM_2     2 // Numero de Sensores do Arduino 2.
#define ARDUINO_2       8 //Endereco do Arduino 2

int arduino_1[SONAR_NUM_1];
int arduino_2[SONAR_NUM_2];

int RainVal;
int IsRaining;
int Temp;
int Hum;
String strRaining;
int v = 0;

int read_LCD_buttons(){               // ler botao
    adc_key_in = analogRead(0);        

    if (adc_key_in > 1000) return btnNONE; 

     if (adc_key_in < 50)   return btnRIGHT;  
     if (adc_key_in < 195)  return btnUP; 
     if (adc_key_in < 380)  return btnDOWN; 
     if (adc_key_in < 555)  return btnLEFT; 
     if (adc_key_in < 790)  return btnSELECT;  

    return btnNONE;                // Se nenhuma acima tiver aciona returna isso.
}

void setup() {

  Wire.begin(); // adiciona o master no barramento I2C
  Serial.begin(115200); 
  lcd.begin(16, 2);               // start the library
  lcd.setCursor(0,0);             // set the LCD cursor   position 
  lcd.print("Push the buttons"); 
}

void loop() {
  ////////// Parte para receber os dados do SLAVE 1 ////////// 
  ////////// Cria um vetor dos sensores do Arduino 1 ////////// 
  
  Wire.requestFrom(ARDUINO_1, SONAR_NUM_1);    // requisita um dado do dispositivo slave com endereço ARDUINO_1
  
  Serial.println();
  Serial.println("///////////////   Dados do Arduino 1 ///////////////");
  
  int i = 0;
  while (Wire.available()) { // o endereço pode mandar menos que o requisitado
    for(i = 0; i< SONAR_NUM_1; i++){
      int c = Wire.read(); // recebe o byte como inteiro
      arduino_1[i] = c;
    }
    Temp = arduino_1[0];
    Hum = arduino_1[1];
    IsRaining = arduino_1[2];
    RainVal = arduino_1[3]*4;
    Serial.print("Temp: ");
    Serial.print(Temp);
    Serial.print("        Humidade: ");
    Serial.print(Hum);
    Serial.print("        Chuvendo: ");
    if(IsRaining){
    strRaining = "S";
    }
    else{
      strRaining = "N";
    }
    Serial.print(strRaining);
    Serial.print("        Nivel de Agua: ");
    Serial.println(RainVal);
  }

  ////////// Parte para receber os dados do SLAVE 2 ////////// 
  ////////// Cria um vetor dos sensores do Arduino 2 ////////// 
  
  Wire.requestFrom(ARDUINO_2, SONAR_NUM_2);    // requisita um dado do dispositivo slave com endereço ARDUINO_2
  
  Serial.println();
  Serial.println("///////////////   Dados do Arduino 2 ///////////////");
  
  while (Wire.available()) { // o endereço pode mandar menos que o requisitado
    for(i = 0; i< SONAR_NUM_2; i++){
      int c = Wire.read(); // recebe o byte como inteiro
      arduino_2[i] = c;
      Serial.print("Sensor Distancia ");
      Serial.print(i);
      Serial.print(": ");
      Serial.println(arduino_2[i]);
    }
  }

  delay(100);

  /////// LCD ///////

  analogWrite (10, fadeValue);    

  lcd_key = read_LCD_buttons();
  switch (lcd_key){
        case btnRIGHT:{             //  Aperte o botao direito ou esquerda para trocar o vetor do Arduino que vai ser mostrado no LCD
            if(v == 0) {
              lcd.setCursor(0,0);
              lcd.print("Arduino 2");
            }
            if(v == 1) {
              lcd.setCursor(0,0);
              lcd.print("Arduino 1");
            }
            delay(500);
            lcd.clear(); // Limpa a tela do LCD.
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
            lcd.clear(); // Limpa a tela do LCD.
            v++;
            break;
       }   
  }

  if(v > 1){ //se v for maior que 1, v sera = 0
    v = 0;
  }
  
  //Dados do Arduino 1 - Estacaoo Meteorologica
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
    lcd.print("Chuva ");
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

  //Dados do Arduino 2 - Mulsensores Distancia
  if(v == 1){ 
    lcd.setCursor(0,0);
    lcd.print("Distancia 1: ");
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
    lcd.print("Distancia 2: ");
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
// MULTISENSORES - MASTER + LCD - POR I2C  Gabriel Nakata - 2016
// ---------------------------------------------------------------------------
