/*
 * SOME INSTRUCTIONS:
 * 
 * Update a float array argument fed into updateMotorPower to move the two links in the arm
 * Update a float argument fed into updateBasePosition to move the base 
 * Set initArmDown to true to move the arm to the datum position before any operation
 * 
 * The position of the two arm links can be read from encoderAngle, a float array of size 2
 * The position of the base can be read from baseAngle. 
 * 
 * The angular velocities of two links can be read from angularVelocity but it's a bit dodgy at the moment.
 */

#include "config.h"
#include "controller.h"

// Settings
bool initArmDown = true;

void setup() {

  Serial.begin(115200);
  
  pinSetting();
  enableAllMotors();
}

void loop() {

  delay(1000);
  if(initArmDown == true) {
    // move arm down here 
    moveToZero();

    delay(3000);
  }

  // resetting some timers here
  mainTime = millis();
  loopCountTime = micros();
  printTime = millis();
  long incramentTime = 4000; // for testing, not relevant for operation

  float armDemand[2] = {0, 0}; // demand for the two arm motors - in degrees
  float baseDemand = 0;  // demand for the base motor - in degrees
  
 

  // main loop
  while(true) {
    
    updateBasePosition(baseDemand);
    updateMotorPower(armDemand);
    //printStuff();  // uncomment and add what you want to print here
  }
  

}
