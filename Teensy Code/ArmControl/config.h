#pragma once

/*
 *  PINS
 */

// Motors
#define M1_INA 24
#define M1_INB 36
#define M1_PWM 2
#define M1_EN 7

const int mainLink_INA[2] = {25, 38};
const int mainLink_INB[2] = {37, 26};
const int mainLink_PWM[2] = {3, 4};

#define M2_EN 8
#define M3_EN 9

// End Switches 
const int openEndStop[2] = {19, 22};
const int closedEndStop[2]  = {20, 21};


// Encoders
#define BASE_POT 33

#define ENCODER_S_1 31
#define ENCODER_S_2 32
#define ENCODER_E_1 29
#define ENCODER_E_2 30


/*
 *  config parameters
 */

#define reverse 0
#define forward 1
#define right 0
#define left 1

/*
 *  config functions
 */
void pinSetting();
void enableAllMotors();
