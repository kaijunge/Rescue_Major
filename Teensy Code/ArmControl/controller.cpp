#include <Arduino.h>
#include "controller.h"

bool endSwitchFlag[2] = {true, true};
bool dir[2] = {true, true};
bool storedEncoderState[2] = {true, true};
bool currentEncoderState[2] = {true, true};
bool endStopState[2] = {false, false};
bool velocityFlag[2] = {false, false};
bool accelerationFlag[2] = {false, false};
bool baseDir;
bool baseDatum = false;

int rawEncoderCount[2] = {0,0};

float encoderAngle[2] = {0,0};
float prevEncoderAngle[2] = {0,0};
float encoderError[2] = {0,0};
float velocityError[2] = {0,0};
float accelerationError[2] = {0,0};
float power[2] = {0,0};
float basePow = 0;
float prevPower[2] = {0,0};
float angularVelocity[2] = {0,0};
float prevAngularVelocity[2] = {0,0};
float angularAcceleration[2] = {0,0};
float demandVelocity[2] = {0,0};
float baseAngle = 0;
float baseError = 0;
float baseAngleCutoff = 90;


long deltaT = 0;
long loopCountTime;
long printTime;
long mainTime;


const float kp = 3.7;           //proportional controller gain
const float kv_max = 10000;       //maximum velocity controller gain
const float kv = 5000;           //generic velocity controller gain
const float ka_max = 1000;
const float kb = 30;
const float basePower = 50;   
const float kpow = 0.5;
const float rawToReal = 0.010588; // change this if sampling rate drops...
const float velMeasureTime = 500; // MICROSECONDS!!
const float maxVel = 0.04;
const float maxAcc = 0.01;
const float targetVel[2] = {0.01, 1};
const float basePotScale = 0.439; // change this later
const int endStopBreakTime = 20;
const int printMillis = 50;
const int basePotOffset = 490;
const int basePowLowThreshold = 10;


//internal settings (for the time being)
bool velControl = false; // not optimised yet!
bool maxVelCutoff = true; 
bool maxAccCutoff = true;

// counting the ticks for the shoulder and elbow motors via interrupts
void shoulderCount() {
  dir[0] == forward ?  rawEncoderCount[0] += 1 : rawEncoderCount[0] -= 1;
}

void elbowCount() {
  dir[1] == forward ?  rawEncoderCount[1] += 1 : rawEncoderCount[1] -= 1;
}

// Print errors and stuff 
void printStuff(){
  if(millis() - printTime > printMillis) {
    // print stuff here

    Serial.print("Pot power: "); Serial.print(basePow); Serial.print("  ");
    Serial.print("Pot error: "); Serial.print(baseError); Serial.print("  ");
    Serial.print("Pot dir: "); Serial.print(baseDir); Serial.print("  ");
    
    Serial.println(" "); 
    printTime = millis();
  }
}

// find velocity setting
void findVelSetting(float *demandAngle) {
  const float minVel = 0.025;
  const float maxVel = 0.5;
  const float correctionExponent = 1.5;
  float moveRange[2] = {abs(demandAngle[0] - encoderAngle[0]),abs(demandAngle[1] - encoderAngle[1])};

  if(moveRange[0] > moveRange[1]) {
    demandVelocity[0] = maxVel;
    demandVelocity[1] = maxVel * (moveRange[1]/pow(moveRange[0],correctionExponent));
  }
  else {
    demandVelocity[1] = maxVel;
    demandVelocity[0] = maxVel * (moveRange[0]/pow(moveRange[1],correctionExponent));
  }

  for(int i = 0; i<2; i++) {
    if(demandVelocity[i] < minVel) {
      demandVelocity[i] = minVel;
    }
  }

  Serial.print("demand Velocity:  "); Serial.print(demandVelocity[0]); Serial.print("    "); Serial.println(demandVelocity[1]);
}

// Update power and direction for base motor
void updateBasePosition(float demandAngle, bool datum) {
  baseDir = left;
  
  baseAngle = (analogRead(BASE_POT) - basePotOffset)*basePotScale;
  baseError = demandAngle - baseAngle;

  basePow = abs(baseError) * kb; 

  if(basePow < basePowLowThreshold) {
    basePow = 0;
  }
  else if(basePow > 100) {
    basePow = 100;
  }

  if(baseError > 0) {
    baseDir = right;
  }

  if(datum == true && abs(baseAngle) < 2) {
      digitalWrite(M1_INA, LOW);  digitalWrite(M1_INB, LOW);  analogWrite(M1_PWM, 0);
      baseDatum = true;
  }
  else {
      if((baseAngle > baseAngleCutoff && baseDir == right) || (baseAngle < -1*baseAngleCutoff && baseDir == left)){
        digitalWrite(M1_INA, LOW);  digitalWrite(M1_INB, LOW);  analogWrite(M1_PWM, 0);
      }
      else {
        digitalWrite(M1_INA, baseDir? LOW : HIGH);  digitalWrite(M1_INB, baseDir? HIGH : LOW);  analogWrite(M1_PWM, 255 * (basePow/100)); 
      }
  }
}




// Update power and direction
void updateMotorPower(float *demandAngle) {

  // Velocity and acceleration monitoring
   if(micros() - loopCountTime > velMeasureTime) {
    deltaT = (micros() - loopCountTime); 
    loopCountTime = micros();

       for(int i = 0; i<2; i++) {
          // at this point do some velocity magic
          // angular velocity = angle/microsec
          angularVelocity[i] = abs(encoderAngle[i] - prevEncoderAngle[i])/(velMeasureTime / 1000);  // convert to milliseconds
          prevEncoderAngle[i] = encoderAngle[i];

          angularAcceleration[i] = abs(angularVelocity[i] - prevAngularVelocity[i])/(velMeasureTime / 1000);  // convert to milliseconds
          prevAngularVelocity[i] = angularVelocity[i];
       }
    }
    
  // Position and velocity feedback loop
  for(int i = 0; i< 2; i++) {
    //if((digitalRead(openEndStop[i]) != 0 || dir[i] != forward)&& (digitalRead(closedEndStop[i]) != 0 && dir[i] != reverse)) {
        encoderAngle[i] = rawEncoderCount[i] * rawToReal; 
        encoderError[i] = demandAngle[i] - encoderAngle[i];
      
        power[i] = abs(kp * encoderError[i]);
    
        dir[i] = forward;
        if(encoderError[i] < 0) {
              dir[i] = reverse;
        }
      
        if(power[i] > basePower) {
          power[i] = basePower;
        }


        // control for the velocity 
        // maxVelCutoff = if velocity is larger than max, it will reduce it
        // velControl = try to keep it at a constant constant velocity - not sure if good idea.. 
        if(maxVelCutoff == true && angularVelocity[i] > maxVel) {
           velocityError[i] = angularVelocity[i] - maxVel; 
           power[i] -= kv_max*velocityError[i];
           velocityFlag[i] = true;
        }
        else if(velControl == true && abs(encoderError[i]) > 5) {
           velocityError[i] = demandVelocity[i] - angularVelocity[i]; 
           power[i] += kv*velocityError[i];
           velocityFlag[i] = true;
        }
        else {
          velocityFlag[i] = false; 
        }


        // control for the acceleration
        if(maxAccCutoff == true && angularAcceleration[i] > maxAcc) {
           accelerationError[i] = angularAcceleration[i] - maxAcc; 
           power[i] -= ka_max*accelerationError[i];
           accelerationFlag[i] = true;
        }
        else {
          accelerationFlag[i] = false; 
        }
        
  }



  
   
   
  // check to see if endstops have been hit 
  for(int i = 0; i<2; i++) {
      
      if(digitalRead(openEndStop[i]) == 0 && dir[i] == forward){
        if(endStopState[i] == false) {
          digitalWrite(mainLink_INA[i], LOW); digitalWrite(mainLink_INB[i], HIGH); analogWrite(mainLink_PWM[i], 255); delay(endStopBreakTime); Serial.println("Hit Endstop");
          endStopState[i] = true;
        }
      }
      else if(digitalRead(closedEndStop[i]) == 0 && dir[i] == reverse){
        if(endStopState[i] == false) {
         
          digitalWrite(mainLink_INA[i], HIGH); digitalWrite(mainLink_INB[i], LOW); analogWrite(mainLink_PWM[i], 255); delay(endStopBreakTime); Serial.println("Hit Endstop");
          endStopState[i] = true;
        }
      }
      else{
        endStopState[i] = false;
      }

      if(endStopState[i] == true) {
        digitalWrite(mainLink_INA[i], LOW);  digitalWrite(mainLink_INB[i], LOW);  analogWrite(mainLink_PWM[i], 0);
      }
      else {
        if(power[i] > 100) {
          power[i] = 100;
        }
        else if(power[i] < 0) {
          power[i] = 0;
        }
        
        digitalWrite(mainLink_INA[i], dir[i]? HIGH : LOW);  digitalWrite(mainLink_INB[i], dir[i]? LOW : HIGH);  analogWrite(mainLink_PWM[i], 255 * (power[i]/100));
      }
  }
}

// move the arm to the zero position
void moveToZero() {
  float zeroDemand[2] = {-99999, -99999};
  findVelSetting(zeroDemand);
  while(baseDatum == false) {
    updateBasePosition(0, true);
  }
  delay(2000);
  while(true) {
      updateMotorPower(zeroDemand);
      updateMotorPower(zeroDemand);
      if(endStopState[0] == true && endStopState[1] == true) {
        break;
      }
  }

  rawEncoderCount[0] = 0;
  rawEncoderCount[1] = 0;

  Serial.print("\n\n ZERO POSITION REACHED \n\n");
  
}
