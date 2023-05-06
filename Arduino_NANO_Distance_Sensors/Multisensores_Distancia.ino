// ---------------------------------------------------------------------------
// MULTISENSORES - Sensores de Distância - POR I2C  Gabriel Nakata - 2016
// ---------------------------------------------------------------------------

#include <Wire.h>
#include <NewPing.h>

#define SONAR_NUM_2     2 // Number of Sensors
#define ARDUINO_2       8 // Arduino Address

#define MAX_DISTANCE 200 // Max Distance in cm.
#define PING_INTERVAL 33 // Milisegunds between the pings

unsigned long pingTimer[SONAR_NUM_2]; 
unsigned int cm[SONAR_NUM_2];         // Storage the Distances 
uint8_t currentSensor = 0;          // Check which sensor is activated
 
NewPing sonar[SONAR_NUM_2] = {  
  NewPing(7, 8, MAX_DISTANCE), // Trigger / Echo / Max Distance
  NewPing(5, 6, MAX_DISTANCE)  // Trigger / Echo / Max Distance
};


void setup() {
  Serial.begin(115200);
  pingTimer[0] = millis() + 75;
  for (uint8_t i = 1; i < SONAR_NUM_2; i++)
    pingTimer[i] = pingTimer[i - 1] + PING_INTERVAL;
  
  Wire.begin(ARDUINO_2);                // Start I2C with the address
  Wire.onRequest(requestEvent); // if the MASTER request data, it will do the requestEvent
  
}

void loop() {
  for (uint8_t i = 0; i < SONAR_NUM_2; i++) { // Loop in the Sensors
    if (millis() >= pingTimer[i]) {         
      pingTimer[i] += PING_INTERVAL * SONAR_NUM_2;  
      if (i == 0 && currentSensor == SONAR_NUM_2 - 1) oneSensorCycle(); // After end cycle, it does something with the data
      sonar[currentSensor].timer_stop();          // For safety
      currentSensor = i;                         
      cm[currentSensor] = 0;                      // Distance is zero when is not detecting anything
      sonar[currentSensor].ping_timer(echoCheck); // Ping
    }
  }
  
}

void echoCheck() {  // get the ping of the sensor
  if (sonar[currentSensor].check_timer())
    cm[currentSensor] = sonar[currentSensor].ping_result / US_ROUNDTRIP_CM;
}

void oneSensorCycle() { // After end cycle, it does something with the data
  for (uint8_t i = 0; i < SONAR_NUM_2; i++) {
    Serial.print(i);
    Serial.print("=");
    Serial.print(cm[i]);
    Serial.print("cm ");
  }
  Serial.println();
}

void requestEvent(){
  
  uint8_t data[SONAR_NUM_2];  //data to be sent to MASTER
  for(int i = 0; i < SONAR_NUM_2; i++)
  {
    data[i] = cm[i];
  }
  
  Wire.write(data,SONAR_NUM_2); // vector, bytes numbers to send
} 

// ---------------------------------------------------------------------------
// MULTISENSORES - Sensores de Distância - POR I2C  Gabriel Nakata - 2016
// ---------------------------------------------------------------------------

