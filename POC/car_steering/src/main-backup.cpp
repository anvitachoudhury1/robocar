

// #include <Arduino.h>
// #include <Servo.h>

// // ---------- RLS-05 Digital Output Pins ----------
// // S0(left)=4, S1(main-left)=5, S2(main/center)=A1, S3(main-right)=7, S4(right)=8
// const int sensorPins[5] = {4, 5, A1, 7, 8};

// // ---------- Servo ----------
// const int servoPin = 3;
// Servo myServo;

// const int centerAngle = 90;
// const int maxSwing = 20; // servo moves centerAngle +/- maxSwing

// // Position weights: leftmost sensor = -2, center = 0, rightmost = +2
// const int weights[5] = {1, 0, 0, 0, -1};

// int lastAngle = centerAngle;

// // ---------- L298N Motor Driver Pins (single motor) ----------
// const int ENA = 6; // Motor speed (PWM) -- moved from pin 9 to avoid Servo/Timer1 conflict
// const int IN1 = 11;
// const int IN2 = 12;

// // ---------- Speed Settings ----------
// const int searchSpeed = 100; // slow speed until line is found
// const int cruiseSpeed = 160; // normal speed once line is detected

// // ---------- Brake state ----------
// bool isBreaked = false;
// bool isInBackGear = false;

// void driveForward(int speed)
// {
//     digitalWrite(IN1, HIGH);
//     digitalWrite(IN2, LOW);
//     analogWrite(ENA, speed);
//     isBreaked = false;
// }

// void breakCar()
// {
//     if (!isBreaked)
//     {
//         // brief reverse pulse for a true stop
//         analogWrite(ENA, 0);
//         digitalWrite(IN1, LOW);
//         digitalWrite(IN2, HIGH);

//         isBreaked = true;
//         analogWrite(ENA, searchSpeed);
//         delay(100);
//     }
//     analogWrite(ENA, 0);
// }

// void backGrear()
// {

//     // brief reverse pulse for a true stop
//     analogWrite(ENA, 0);
//     digitalWrite(IN1, LOW);
//     digitalWrite(IN2, HIGH);

//     isInBackGear = true;
//     isBreaked = false;
//     analogWrite(ENA, searchSpeed);
//     delay(50);
// }
// void setup()
// {
//     // --- set motor pins as OUTPUT before using them ---
//     pinMode(ENA, OUTPUT);
//     pinMode(IN1, OUTPUT);
//     pinMode(IN2, OUTPUT);

//     for (int i = 0; i < 5; i++)
//     {
//         pinMode(sensorPins[i], INPUT);
//     }

//     driveForward(searchSpeed);

//     myServo.attach(servoPin);
//     myServo.write(centerAngle);

//     Serial.begin(9600);
// }

// void loop()
// {
//     int weightedSum = 0;
//     int activeCount = 0;
//     int sensorValues[5];

//     for (int i = 0; i < 5; i++)
//     {
//         sensorValues[i] = digitalRead(sensorPins[i]);
//         if (sensorValues[i] == LOW)
//         { // line detected
//             weightedSum += weights[i];
//             activeCount++;
//         }
//     }

//     // Middle 3 sensors: S1 (main-left), S2 (main/center), S3 (main-right)
//     int sensorMainLeftValue = sensorValues[1];
//     int sensorMainPinValue = sensorValues[2];
//     int sensorMainRightValue = sensorValues[3];
//     int motorSpeed = 0;
//     // ---- Servo steering ----
//     int angle;
//     if (activeCount == 0)
//     {
//         angle = lastAngle; // hold last angle if line not seen
//     }
//     else
//     {
//         float avgPosition = (float)weightedSum / activeCount;
//         angle = centerAngle + (int)(avgPosition * (maxSwing / 2.0));
//         angle = constrain(angle, centerAngle - maxSwing, centerAngle + maxSwing);
//         lastAngle = angle;
//     }
//     myServo.write(angle);

//     // ---- Stop condition: all 3 middle sensors agree (all line, or all no line) ----
//     if ((sensorMainPinValue == 0 && sensorMainLeftValue == 0 && sensorMainRightValue == 0) ||
//         (sensorMainPinValue == 1 && sensorMainLeftValue == 1 && sensorMainRightValue == 1))
//     {
//         if (!isBreaked)
//         {
//             breakCar();
//         }
//         else
//         {
//             // backGrear();
//         }
//     }
//     else
//     {
//         isInBackGear = false;
//         // ---- Motor speed: slow until line found, then cruise ----
//         motorSpeed = (activeCount > 0) ? cruiseSpeed : searchSpeed;
//         driveForward(motorSpeed);
//     }

//     // Debug output
//     for (int i = 0; i < 5; i++)
//     {
//         Serial.print(sensorValues[i]);
//         Serial.print(" ");
//     }
//     Serial.print("-> Angle: ");
//     Serial.print(angle);
//     Serial.print(" Speed: ");
//     Serial.print(motorSpeed);
//     Serial.print(" Braked: ");
//     Serial.print(isBreaked);
//     Serial.print(" isInBackGear: ");
//     Serial.println(isInBackGear);

//     delay(50);
// }