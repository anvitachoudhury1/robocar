#include <Arduino.h>
#include "config.h"
#include "led_control.h"
#include "network.h"

static bool ledState = false;

// Non-blocking blink state machine
static bool blinking = false;
static int blinkCount = 0;
static unsigned long lastBlinkTime = 0;
static bool blinkLedOn = false;

void ledInit() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  ledState = false;
}

void ledOn() {
  ledState = true;
  if (!blinking) {
    digitalWrite(LED_BUILTIN, HIGH);
  }
}

void ledOff() {
  ledState = false;
  if (!blinking) {
    digitalWrite(LED_BUILTIN, LOW);
  }
}

bool ledIsOn() {
  return ledState;
}

bool ledStartBlink() {
  if (blinking) return false;  // already in progress

  blinking = true;
  blinkCount = 0;
  blinkLedOn = false;
  lastBlinkTime = millis();
  return true;
}

bool ledIsBlinking() {
  return blinking;
}

void ledUpdate() {
  if (!blinking) return;

  if (millis() - lastBlinkTime >= BLINK_INTERVAL_MS) {
    lastBlinkTime = millis();
    blinkLedOn = !blinkLedOn;
    digitalWrite(LED_BUILTIN, blinkLedOn ? HIGH : LOW);

    if (!blinkLedOn) {
      blinkCount++;
      if (blinkCount >= BLINK_REPEATS) {
        blinking = false;
        // Restore the persistent LED state from before the blink
        digitalWrite(LED_BUILTIN, ledState ? HIGH : LOW);
        broadcastAll("[Server] Blink done");
      }
    }
  }
}
