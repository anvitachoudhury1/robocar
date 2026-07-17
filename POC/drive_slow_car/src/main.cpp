/*
  Basic Servo Motor Program
  Arduino Uno + Servo Motor (no motor driver needed)

  Sweeps the servo back and forth between 0 and 180 degrees.
*/

#include <Arduino.h>
#include <Servo.h>

const int servoPin = 3; // signal wire to any PWM pin
Servo myServo;

void setup() {
  myServo.attach(servoPin);
}

void loop() {
  // Sweep from 0 to 180 degrees
  for (int angle = 0; angle <= 180; angle++) {
    myServo.write(angle);
    delay(15); // small delay controls sweep speed
  }

  // Sweep back from 180 to 0 degrees
  for (int angle = 180; angle >= 0; angle--) {
    myServo.write(angle);
    delay(15);
  }
}
