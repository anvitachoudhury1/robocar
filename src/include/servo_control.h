#ifndef SERVO_CONTROL_H
#define SERVO_CONTROL_H

#include <Arduino.h>

void servoInit();
void servoSetAngle(int angle); // 0-180 degrees
int servoGetAngle();
void servoSweep(int fromAngle, int toAngle, int stepDelayMs);

#endif