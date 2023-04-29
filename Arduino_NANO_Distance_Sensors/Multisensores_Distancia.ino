// ---------------------------------------------------------------------------
// MULTISENSORES - Sensores de Distância - POR I2C  Gabriel Nakata - 2016
// ---------------------------------------------------------------------------

#include <Wire.h>
#include <NewPing.h>

#define SONAR_NUM_2     2 // Numero de Sensores do Arduino 2.
#define ARDUINO_2       8 // Endereco do Arduino 2

#define MAX_DISTANCE 200 // Distancia maxima em cm.
#define PING_INTERVAL 33 // Milisegundos entre os pings dos sensores

unsigned long pingTimer[SONAR_NUM_2]; 
unsigned int cm[SONAR_NUM_2];         // Aonde as distancias sao armazenadas
uint8_t currentSensor = 0;          // verifica qual e o sensor ativo
 
NewPing sonar[SONAR_NUM_2] = {     // Vetor de Sensores. Trigger pin / Echo Pin
  NewPing(7, 8, MAX_DISTANCE), // Trigger / Echo / Maxima Distância
  NewPing(5, 6, MAX_DISTANCE)  // Trigger / Echo / Maxima Distância
};


void setup() {
  Serial.begin(115200); // 
  pingTimer[0] = millis() + 75;           // O primeiro ping começa em 75ms, dando um tempo para o Arduino inicializar
  for (uint8_t i = 1; i < SONAR_NUM_2; i++) // Seta o tempo de inicio de cada sensore
    pingTimer[i] = pingTimer[i - 1] + PING_INTERVAL;
  
  Wire.begin(ARDUINO_2);                // inicia o i2c com o endereco ARDUINO_2
  Wire.onRequest(requestEvent); // caso seja solicitado este slave, ele vai para a funcao que foi atribuido
  
}

void loop() {
  for (uint8_t i = 0; i < SONAR_NUM_2; i++) { // Da um loop em todos os sensores
    if (millis() >= pingTimer[i]) {         
      pingTimer[i] += PING_INTERVAL * SONAR_NUM_2;  // seta o proximo ttempo que este sensor ira pingar
      if (i == 0 && currentSensor == SONAR_NUM_2 - 1) oneSensorCycle(); // Ciclo completo, faca alguma coisa com os resultados
      sonar[currentSensor].timer_stop();          // Para seguranca
      currentSensor = i;                         
      cm[currentSensor] = 0;                      // Faz a distancia ser zero caso não tenho ping para este sensor
      sonar[currentSensor].ping_timer(echoCheck); // Faz o ping
    }
  }
  //Adicione aqui o que você quer caso queira adicionar algo no programa que nao tenha haver com o sensor de distancia
}

void echoCheck() { // Se receber ping, seta a distancia do sensor no vetor
  if (sonar[currentSensor].check_timer())
    cm[currentSensor] = sonar[currentSensor].ping_result / US_ROUNDTRIP_CM;
}

void oneSensorCycle() { // Ciclo completo, faca alguma coisa com os resultados
  for (uint8_t i = 0; i < SONAR_NUM_2; i++) {
    Serial.print(i);
    Serial.print("=");
    Serial.print(cm[i]);
    Serial.print("cm ");
  }
  Serial.println();
}

void requestEvent(){
  
  uint8_t data[SONAR_NUM_2];  //vetor para envio de dados para o MASTER
  for(int i = 0; i < SONAR_NUM_2; i++)
  {
    data[i] = cm[i];
  }
  
  Wire.write(data,SONAR_NUM_2); // vetor, numero de bytes para enviar
} 

// ---------------------------------------------------------------------------
// MULTISENSORES - Sensores de Distância - POR I2C  Gabriel Nakata - 2016
// ---------------------------------------------------------------------------

