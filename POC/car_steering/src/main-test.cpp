

// #include <Arduino.h>
// #include <Servo.h>

// const int sensorRightPin = 5;
// const int sensorMainPin = A1;
// const int sensorLeftPin = 7;
// // // ---------- RLS-05 Digital Output Pins ----------
// // // S0(left)=4, S1=5, S2(center)=A1, S3=7, S4(right)=8
// // const int sensorPins[5] = {4, 5, A1, 7, 8};

// // ---------- Servo ----------
// const int servoPin = 3;
// Servo myServo;

// const int centerAngle = 90;
// const int maxSwing = 20; // servo moves centerAngle +/- maxSwing

// // Position weights: leftmost sensor = -2, center = 0, rightmost = +2
// // const int weights[5] = {2, 1, 0, -1, -2};

// int lastAngle = centerAngle;

// // ---------- L298N Motor Driver Pins (single motor) ----------
// const int ENA = 6; // Motor speed (PWM) -- moved from pin 9 to avoid Servo/Timer1 conflict
// const int IN1 = 11;
// const int IN2 = 12;

// int isBreaked = 0;
// // ---------- Speed Settings ----------
// const int searchSpeed = 160; // slow speed until line is found
// const int cruiseSpeed = 250; // normal speed once line is detected

// void driveForward(int speed)
// {
//   digitalWrite(IN1, HIGH);
//   digitalWrite(IN2, LOW);
//   analogWrite(ENA, speed);
//   if (speed != 0)
//   {
//     isBreaked = 0;
//   }
// }

// void breakCar()
// {
//   if (!isBreaked)
//   {
//     analogWrite(ENA, 0);
//     digitalWrite(IN1, LOW);
//     digitalWrite(IN2, HIGH);

//     isBreaked = 1;
//     analogWrite(ENA, searchSpeed);
//     delay(50);
//   }
//   analogWrite(ENA, 0);
// }

// void setup()
// {
//   // --- set motor pins as OUTPUT before using them ---
//   pinMode(ENA, OUTPUT);
//   pinMode(IN1, OUTPUT);
//   pinMode(IN2, OUTPUT);

//   pinMode(sensorMainPin, INPUT);
//   pinMode(sensorRightPin, INPUT);
//   pinMode(sensorLeftPin, INPUT);

//   // for (int i = 0; i < 5; i++) {
//   //   pinMode(sensorPins[i], INPUT);
//   // }

//   // Fixed forward direction
//   digitalWrite(IN1, HIGH);
//   digitalWrite(IN2, LOW);

//   // driveForward(searchSpeed);

//   myServo.attach(servoPin);
//   myServo.write(centerAngle);

//   Serial.begin(9600);
// }

// void loop()
// {
//   // int weightedSum = 0;
//   // int activeCount = 0;
//   int motorSpeed = cruiseSpeed;
//   int sensorMainPinValue = digitalRead(sensorMainPin);
//   int sensorMainRightValue = digitalRead(sensorRightPin);
//   int sensorMainLeftValue = digitalRead(sensorLeftPin);

//   if ((sensorMainPinValue == 0 && sensorMainLeftValue == 0 && sensorMainRightValue == 0) ||
//       (sensorMainPinValue == 1 && sensorMainLeftValue == 1 && sensorMainRightValue == 1))
//   {
//     motorSpeed = 0;
//     breakCar();
//   }
//   else if (sensorMainPinValue == 0 || sensorMainLeftValue == 0 || sensorMainRightValue == 0)
//   {
//     motorSpeed = cruiseSpeed;
//   }
//   else
//   {
//     motorSpeed = 0;
//     breakCar();
//   }
//   driveForward(motorSpeed);

//   Serial.print("sensorMainPinValue:");
//   Serial.print(sensorMainPinValue);
//   Serial.print("  sensorMainLeftValue:");
//   Serial.print(sensorMainLeftValue);

//   Serial.print(" sensorMainRightValue:");
//   Serial.print(sensorMainRightValue);
//   Serial.print(" Speed: ");
//   Serial.println(motorSpeed);

//   // for (int i = 0; i < 5; i++) {
//   //   int value = digitalRead(sensorPins[i]);
//   //   if (value == LOW) { // line detected
//   //     weightedSum += weights[i];
//   //     activeCount++;

//   //   }
//   // }

//   // // ---- Servo steering ----
//   // int angle;
//   // if (activeCount == 0) {
//   //   angle = lastAngle; // hold last angle if line not seen
//   // } else {
//   //   float avgPosition = (float)weightedSum / activeCount;
//   //   angle = centerAngle + (int)(avgPosition * (maxSwing / 2.0));
//   //   angle = constrain(angle, centerAngle - maxSwing, centerAngle + maxSwing);
//   //   lastAngle = angle;
//   // }
//   // myServo.write(angle);

//   // // ---- Motor speed: slow until line found, then cruise ----
//   // int motorSpeed = (activeCount > 0) ? cruiseSpeed : searchSpeed;
//   // driveForward(motorSpeed);

//   // // Debug output
//   // for (int i = 0; i < 5; i++) {
//   //   Serial.print(digitalRead(sensorPins[i]));
//   //   Serial.print(" ");
//   // }
//   // Serial.print("-> Angle: ");
//   // Serial.print(angle);
//   // Serial.print(" Speed: ");
//   // Serial.println(motorSpeed);

//   delay(50);
// }