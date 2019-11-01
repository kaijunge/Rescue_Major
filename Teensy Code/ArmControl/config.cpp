#include <Arduino.h>
#include "config.h"

// Pin mode setting for all IO pins
void pinSetting() {
  pinMode(openEndStop[0], INPUT);
  pinMode(openEndStop[1], INPUT);
  pinMode(closedEndStop[0], INPUT);
  pinMode(closedEndStop[1], INPUT);
  
  pinMode(ENCODER_S_1, INPUT);
  pinMode(ENCODER_S_2, INPUT);
  pinMode(ENCODER_E_1, INPUT);
  pinMode(ENCODER_E_2, INPUT);
  
  pinMode(M1_INA, OUTPUT);
  pinMode(M1_INB, OUTPUT);
  
  pinMode(mainLink_INA[0], OUTPUT);
  pinMode(mainLink_INB[0], OUTPUT);
  
  pinMode(mainLink_INA[1], OUTPUT);
  pinMode(mainLink_INB[1], OUTPUT);

}


void enableAllMotors() {
  digitalWrite(M1_EN, HIGH);
  digitalWrite(M2_EN, HIGH);
  digitalWrite(M3_EN, HIGH);

  digitalWrite(M1_INA, LOW);
  digitalWrite(M1_INB, LOW);

  digitalWrite(mainLink_INA[0], LOW);
  digitalWrite(mainLink_INB[0], LOW);

  digitalWrite(mainLink_INA[1], LOW);
  digitalWrite(mainLink_INB[1], LOW);
}
