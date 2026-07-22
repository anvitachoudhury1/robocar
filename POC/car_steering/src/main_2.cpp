// // /*
// //   Simple Motor Start using L298N
// //   Arduino Uno + L298N Motor Driver
// //   Just starts both motors forward at a fixed speed. Nothing else.
// // */

// // #include <Arduino.h>

// // // ---------- L298N Motor Driver Pins ----------
// // const int ENA = 9;   // Left motor speed (PWM)
// // const int IN1 = 11;
// // const int IN2 = 12;
// // const int IN3 = 13;
// // const int IN4 = A0;
// // const int ENB = 10;  // Right motor speed (PWM)

// // const int motorSpeed = 150; // 0-255

// // void setup() {
// //   pinMode(ENA, OUTPUT);
// //   pinMode(IN1, OUTPUT);
// //   pinMode(IN2, OUTPUT);
// //   pinMode(IN3, OUTPUT);
// //   pinMode(IN4, OUTPUT);
// //   pinMode(ENB, OUTPUT);

// //   // Set direction: forward
// //   digitalWrite(IN1, HIGH);
//   digitalWrite(IN2, LOW);
//   digitalWrite(IN3, HIGH);
//   digitalWrite(IN4, LOW);

// //   // Start motors
//   analogWrite(ENA, motorSpeed);
//   analogWrite(ENB, motorSpeed);
// // }

// // void loop() {
// //   motors just keep running, nothing else happens
// // }