// Smart Device Server — Arduino UNO R4 WiFi
//
// Creates a WiFi Access Point and runs a raw TCP command server (port 23)
// supporting multiple simultaneous clients. Commands from any client are
// executed and results are broadcast to all connected clients.
//
// Connect from a terminal:   nc 192.168.4.1 23
// Commands: led on | led off | status | blink | who | exit
//
// Structure:
//   src/main.cpp        — setup() + loop() orchestration only
//   src/network.cpp     — AP setup, client slots, connect/disconnect, input
//   src/commands.cpp    — command parsing and execution
//   src/led_control.cpp — LED state + non-blocking blink animation
//   include/config.h    — all constants and pin definitions

#include <Arduino.h>
#include "config.h"
#include "network.h"
#include "led_control.h"
#include "heart_control.h"
#include "door_control.h"
void setup()
{
  Serial.begin(9600);

  // Wait briefly for Serial, but don't block forever if no monitor attached
  // (important: with `while (!Serial);` the device would hang when powered
  // from a wall adapter instead of a computer).
  unsigned long start = millis();
  while (!Serial && millis() - start < 3000)
  {
    ;
  }

  ledInit();
  heartInit();
  networkInit();
  doorInit();
}

void loop()
{
  networkUpdate(); // accept clients, detect disconnects, dispatch commands
  ledUpdate();     // advance non-blocking blink animation
}
