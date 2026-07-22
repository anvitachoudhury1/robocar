/*
  Servo Steering + L298N Drive Motor (single motor), SmartElex RLS-05 (5-sensor digital)
  Arduino Uno + L298N Motor Driver + Servo Motor + RLS-05 Line Sensor Array

  IMPORTANT: Calibrate the RLS-05 first using its onboard button
  (press twice for black-line mode, or three times for white-line
  mode) before running this code, or the digital outputs won't be
  reliable.

  Digital output assumed:
    - LOW  = line detected under that sensor
    - HIGH = no line

  Behavior:
    - Motor starts at a slow "search" speed the moment the robot powers on.
    - Once ANY sensor detects the line, motor ramps up to normal cruise speed.
    - Servo continuously steers based on weighted sensor position.
    - If the line is lost again (no sensor sees it) after being found,
      motor drops back to search speed until the line is reacquired.

  PIN NOTE: ENA is on pin 6, NOT pin 9. Pins 9 and 10 share Timer1 with
  the Servo library -- the instant myServo.attach() runs, analogWrite()
  on pins 9/10 stops working correctly. Pin 6 uses Timer0, which Servo
  never touches, so motor PWM and the servo now work at the same time.
  Because pin 6 was previously used by a sensor, that sensor has been
  moved to pin A1 to avoid a conflict.
*/

#include <Arduino.h>
#include <Servo.h>

// ---------- RLS-05 Digital Output Pins ----------
// S0(left)=4, S1=5, S2(center)=A1, S3=7, S4(right)=8
const int sensorPins[5] = {4, 5, A1, 7, 8};

// ---------- Servo ----------
const int servoPin = 3;
Servo myServo;

const int centerAngle = 90;
const int maxSwing = 20; // servo moves centerAngle +/- maxSwing

// Position weights: leftmost sensor = -2, center = 0, rightmost = +2
const int weights[5] = {2, 1, 0, -1, -2};

int lastAngle = centerAngle;

// ---------- L298N Motor Driver Pins (single motor) ----------
const int ENA = 6;   // Motor speed (PWM) -- moved from pin 9 to avoid Servo/Timer1 conflict
const int IN1 = 11;
const int IN2 = 12;

// ---------- Speed Settings ----------
const int searchSpeed = 160;   // slow speed until line is found
const int cruiseSpeed = 255;  // normal speed once line is detected

void driveForward(int speed) {
  analogWrite(ENA, speed);
}

void setup() {
  // --- set motor pins as OUTPUT before using them ---
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  for (int i = 0; i < 5; i++) {
    pinMode(sensorPins[i], INPUT);
  }

  // Fixed forward direction
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  driveForward(searchSpeed);

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

  // ---- Servo steering ----
  int angle;
  if (activeCount == 0) {
    angle = lastAngle; // hold last angle if line not seen
  } else {
    float avgPosition = (float)weightedSum / activeCount;
    angle = centerAngle + (int)(avgPosition * (maxSwing / 2.0));
    angle = constrain(angle, centerAngle - maxSwing, centerAngle + maxSwing);
    lastAngle = angle;
  }
  myServo.write(angle);

  // ---- Motor speed: slow until line found, then cruise ----
  int motorSpeed = (activeCount > 0) ? cruiseSpeed : searchSpeed;
  driveForward(motorSpeed);

  // Debug output
  for (int i = 0; i < 5; i++) {
    Serial.print(digitalRead(sensorPins[i]));
    Serial.print(" ");
  }
  Serial.print("-> Angle: ");
  Serial.print(angle);
  Serial.print(" Speed: ");
  Serial.println(motorSpeed);

  delay(50);
}