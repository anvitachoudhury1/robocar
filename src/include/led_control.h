#ifndef LED_CONTROL_H
#define LED_CONTROL_H

#include <Arduino.h>

// Configure the LED pin. Call once from setup().
void ledInit();

// Turn the onboard LED on/off (updates the persistent state).
void ledOn();
void ledOff();

// Current persistent LED state (the state blink restores to).
bool ledIsOn();

// Start a non-blocking blink sequence (BLINK_REPEATS times).
// Returns false if a blink is already in progress.
bool ledStartBlink();

// True while a blink sequence is running.
bool ledIsBlinking();

// Call every loop() cycle. Advances the blink animation using millis(),
// so the rest of the system stays responsive. Broadcasts "Blink done"
// to all clients when the sequence finishes.
void ledUpdate();

#endif // LED_CONTROL_H
