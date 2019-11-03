#pragma once

#include "config.h"

extern bool endSwitchFlag[2];
extern long loopCountTime;
extern long printTime;
extern long mainTime;
extern float encoderAngle[2];
extern float demandVelocity[2];
extern float baseAngle;
extern float angularVelocity[2];

void count(bool dir[2]);
void printStuff();
void updateMotorPower(float *demandAngle);
void moveToZero();
void findVelSetting(float *demandAngle);
void updateBasePosition(float demandAngle, bool datum = false);
void shoulderCount();
void elbowCount();
