// /*
//   Motor Speed Control Only
//   Arduino Uno + L298N Motor Driver
//   Speed controlled via Serial Monitor input (0-255)
//   Motors always spin in one fixed direction — only speed changes.

//   Usage: Open the PlatformIO Serial Monitor (baud 9600),
//          type a number between 0-255, press Enter.
// */

// #include <Arduino.h>

// // ---------- L298N Motor Driver Pins ----------
// const int ENA = 5;   // Left motor speed (PWM)
// const int IN1 = 6;
// const int IN2 = 7;
// // const int IN3 = 8;
// // const int IN4 = 9;
// // const int ENB = 10;  // Right motor speed (PWM)

// int motorSpeed = 1000; // starts at 0 (stopped)

// void setup() {
//   pinMode(ENA, OUTPUT);
//   pinMode(IN1, OUTPUT);
//   pinMode(IN2, OUTPUT);
//   // pinMode(IN3, OUTPUT);
//   // pinMode(IN4, OUTPUT);
//   // pinMode(ENB, OUTPUT);

//   // Fixed direction, set once
//   digitalWrite(IN1, HIGH);
//   digitalWrite(IN2, LOW);
//   // digitalWrite(IN3, HIGH);
//   // digitalWrite(IN4, LOW);

//   Serial.begin(9600);
//   Serial.println("Enter motor speed (0-255):");
// }

// void loop() {
//   if (Serial.available() > 0) {
//     int newSpeed = Serial.parseInt();

//     Serial.print("Input Speed set to: ");
//       Serial.println(motorSpeed);

//     if(newSpeed == 2){
//        digitalWrite(IN1, LOW);
//        digitalWrite(IN2, HIGH);
//       return;
//     }

//      if(newSpeed == 1){
//        digitalWrite(IN1, HIGH);
//        digitalWrite(IN2, LOW);
//       return;
//     }

//     if (newSpeed >= 0 && newSpeed <= 255) {
//       motorSpeed = newSpeed;
//       Serial.print("Speed set to: ");
//       Serial.println(motorSpeed);
//     } else {
//       Serial.println("Please enter a value between 0 and 255.");
//     }
//   }

//   analogWrite(ENA, motorSpeed);
//   // analogWrite(ENB, motorSpeed);
// }
