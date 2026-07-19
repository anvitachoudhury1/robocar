/*
  Servo Steering using SmartElex RLS-05 (5-sensor digital output)
  Arduino Uno + Servo Motor + SmartElex RLS-05 Line Sensor Array

  IMPORTANT: Calibrate the RLS-05 first using its onboard button
  (press twice for black-line mode, or three times for white-line
  mode) before running this code, or the digital outputs won't be
  reliable. See SmartElex RLS-05 documentation for calibration steps.

  Digital output assumed:
    - LOW  = line detected under that sensor
    - HIGH = no line
  (Swap LOW/HIGH below if your module reports the opposite.)

  Sensor layout (left to right): S0 S1 S2 S3 S4
  Each sensor is given a position weight. The servo angle is
  calculated from the weighted average of which sensors see the line,
  so movement is smooth/proportional instead of snapping between
  fixed positions.
*/

#include <Arduino.h>
#include <Servo.h>

// ---------- RLS-05 Digital Output Pins ----------
const int sensorPins[5] = {4, 5, 6, 7, 8}; // S0 (left) to S4 (right)

// ---------- Servo ----------
const int servoPin = 3;
Servo myServo;

const int centerAngle = 90;
const int maxSwing = 40; // servo moves centerAngle +/- maxSwing

// Position weights: leftmost sensor = -2, center = 0, rightmost = +2
// (Reversed from original so servo turns the correct physical direction)
const int weights[5] = {2, 1, 0, -1, -2};

int lastAngle = centerAngle;

void setup() {
  for (int i = 0; i < 5; i++) {
    pinMode(sensorPins[i], INPUT);
  }

  myServo.attach(servoPin);
  myServo.write(centerAngle);

  Serial.begin(9600);
}

void loop() {
  int weightedSum = 0;
  int activeCount = 0;

  for (int i = 0; i < 5; i++) {
    int value = digitalRead(sensorPins[i]);
    if (value == LOW) { // line detected
      weightedSum += weights[i];
      activeCount++;
    }
  }

  int angle;

  if (activeCount == 0) {
    // No sensor sees the line -> hold last angle
    angle = lastAngle;
  } else {
    float avgPosition = (float)weightedSum / activeCount; // ranges roughly -2 to +2
    angle = centerAngle + (int)(avgPosition * (maxSwing / 2.0));
    angle = constrain(angle, centerAngle - maxSwing, centerAngle + maxSwing);
    lastAngle = angle;
  }

  myServo.write(angle);

  // Debug: print sensor states and resulting angle
  for (int i = 0; i < 5; i++) {
    Serial.print(digitalRead(sensorPins[i]));
    Serial.print(" ");
  }
  Serial.print("-> Angle: ");
  Serial.println(angle);

  delay(50);
}