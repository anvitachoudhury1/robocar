#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>

void motorBegin();

void motorAForward(int speed); // speed: 0-255
void motorABackward(int speed);
void motorAStop();

void motorBForward(int speed);
void motorBBackward(int speed);
void motorBStop();

void motorStopAll();
void motorStandby(bool enable); // true = enabled (STBY HIGH), false = disabled

// Status getters (for "status" command / debugging)
bool motorAIsRunning();
bool motorBIsRunning();
int motorAGetSpeed();
int motorBGetSpeed();
String motorAGetDirection(); // "FORWARD", "BACKWARD", or "STOPPED"
String motorBGetDirection();

#endif