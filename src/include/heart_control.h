#ifndef HEART_CONTROL_H
#define HEART_CONTROL_H

#include <Arduino.h>
// Configure the LED pin. Call once from setup().
void heartInit();

// Turn the onboard LED on/off (updates the persistent state).
void heartOn();
void heartOff();

// Current persistent LED state (the state blink restores to).
bool heartIsOn();
#endif // HEART_CONTROL_H