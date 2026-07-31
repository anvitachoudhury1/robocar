// //==========================================================
// // COMPETITION LINE FOLLOWER
// // Arduino UNO R4
// // main.cpp
// //==========================================================

// #include <Arduino.h>

// //==========================================================
// // SENSOR PINS
// // RLS-05
// // HIGH = BLACK
// //==========================================================

// constexpr uint8_t SENSOR_COUNT = 5;

// constexpr uint8_t sensorPins[SENSOR_COUNT] =
//     {
//         4,
//         5,
//         A1,
//         7,
//         8};

// //==========================================================
// // SENSOR WEIGHTS
// //==========================================================

// constexpr int8_t sensorWeights[SENSOR_COUNT] =
//     {
//         -2,
//         -1,
//         0,
//         1,
//         2};

// //==========================================================
// // MOTOR DRIVER
// // L298N
// //==========================================================

// constexpr uint8_t ENA = 6;
// constexpr uint8_t ENB = 3;

// constexpr uint8_t IN1 = 12;
// constexpr uint8_t IN2 = 11;
// constexpr uint8_t IN3 = 10;
// constexpr uint8_t IN4 = 13;

// // const byte ENA = 6; // Left motor PWM
// // const byte IN1 = 11;
// // const byte IN2 = 12;

// // const byte ENB = 3; // Right motor PWM
// // const byte IN3 = 13;
// // const byte IN4 = 10;

// //==========================================================
// // PID CONSTANTS
// //==========================================================

// float kp = 22.0f;
// float ki = 0.0f;
// float kd = 12.0f;

// //==========================================================
// // PID VARIABLES
// //==========================================================

// float error = 0.0f;
// float previousError = 0.0f;
// float integral = 0.0f;
// float derivative = 0.0f;
// float correction = 0.0f;

// //==========================================================
// // SPEED LIMITS
// //==========================================================

// int baseSpeed = 170;
// int currentBaseSpeed = 170;

// constexpr int MAX_SPEED = 255;
// constexpr int MIN_SPEED = 80;

// int leftMotorSpeed = 0;
// int rightMotorSpeed = 0;

// //==========================================================
// // SENSOR STATE
// //==========================================================

// bool sensorState[SENSOR_COUNT];

// uint8_t activeSensors = 0;

// bool lineDetected = false;

// bool junctionDetected = false;

// bool finishDetected = false;

// //==========================================================
// // LINE POSITION
// //==========================================================

// int weightedSum = 0;

// int sensorSum = 0;

// //==========================================================
// // LOST LINE
// //==========================================================

// bool lastSeenLeft = false;

// bool lastSeenRight = false;

// uint32_t lostStartTime = 0;

// //==========================================================
// // TIMERS
// //==========================================================

// uint32_t debugTimer = 0;

// uint32_t tuningTimer = 0;

// uint32_t finishTimer = 0;

// //==========================================================
// // DEBUG
// //==========================================================

// bool debugEnabled = true;

// //==========================================================
// // RUNTIME FLAGS
// //==========================================================

// bool robotRunning = true;
// bool raceFinished = false;

// //==========================================================
// // SERIAL COMMAND BUFFER
// //==========================================================

// String serialBuffer;

// //==========================================================
// // FUNCTION PROTOTYPES
// //==========================================================

// // Sensors
// void readSensors();
// void calculatePosition();

// // PID
// void updatePID();

// // Speed
// void updateSpeedProfile();

// // Motor Control
// void setMotor(int leftSpeed, int rightSpeed);
// void stopRobot();
// void applyMotorOutput();

// // Line Logic
// void recoverLine();
// void detectJunction();
// void detectFinish();

// // Runtime
// void runtimeTuning();
// void debugOutput();

// // Robot
// void updateRobot();

// //==========================================================
// // SENSOR LAYER
// //==========================================================

// void readSensors()
// {
//     activeSensors = 0;
//     weightedSum = 0;
//     sensorSum = 0;

//     lineDetected = false;

//     for (uint8_t i = 0; i < SENSOR_COUNT; i++)
//     {
//         sensorState[i] = digitalRead(sensorPins[i]);

//         if (sensorState[i]) // HIGH = BLACK
//         {
//             activeSensors++;
//             sensorSum++;
//             weightedSum += sensorWeights[i];
//             lineDetected = true;
//         }
//     }
// }

// //==========================================================

// void calculatePosition()
// {
//     if (lineDetected)
//     {
//         error = (float)weightedSum / (float)sensorSum;

//         previousError = error;

//         if (error < 0.0f)
//         {
//             lastSeenLeft = true;
//             lastSeenRight = false;
//         }
//         else if (error > 0.0f)
//         {
//             lastSeenLeft = false;
//             lastSeenRight = true;
//         }

//         lostStartTime = millis();
//     }
//     else
//     {
//         if (lastSeenLeft)
//         {
//             error = -3.0f;
//         }
//         else if (lastSeenRight)
//         {
//             error = 3.0f;
//         }
//         else
//         {
//             error = previousError;
//         }
//     }
// }

// //==========================================================
// // MOTOR DRIVER LAYER
// //==========================================================

// void setMotor(int leftSpeed, int rightSpeed)
// {
//     leftMotorSpeed = constrain(leftSpeed, -MAX_SPEED, MAX_SPEED);
//     rightMotorSpeed = constrain(rightSpeed, -MAX_SPEED, MAX_SPEED);
// }

// //==========================================================

// void applyMotorOutput()
// {
//     // LEFT MOTOR
//     if (leftMotorSpeed >= 0)
//     {
//         digitalWrite(IN1, HIGH);
//         digitalWrite(IN2, LOW);
//         analogWrite(ENA, leftMotorSpeed);
//     }
//     else
//     {
//         digitalWrite(IN1, LOW);
//         digitalWrite(IN2, HIGH);
//         analogWrite(ENA, -leftMotorSpeed);
//     }

//     // RIGHT MOTOR
//     if (rightMotorSpeed >= 0)
//     {
//         digitalWrite(IN3, HIGH);
//         digitalWrite(IN4, LOW);
//         analogWrite(ENB, rightMotorSpeed);
//     }
//     else
//     {
//         digitalWrite(IN3, LOW);
//         digitalWrite(IN4, HIGH);
//         analogWrite(ENB, -rightMotorSpeed);
//     }
// }

// //==========================================================

// void stopRobot()
// {
//     leftMotorSpeed = 0;
//     rightMotorSpeed = 0;

//     analogWrite(ENA, 0);
//     analogWrite(ENB, 0);

//     digitalWrite(IN1, LOW);
//     digitalWrite(IN2, LOW);
//     digitalWrite(IN3, LOW);
//     digitalWrite(IN4, LOW);
// }

// //==========================================================
// // PID CONTROLLER
// //==========================================================

// void updatePID()
// {
//     // Integral term
//     integral += error;

//     // Prevent integral windup
//     integral = constrain(integral, -100.0f, 100.0f);

//     // Derivative term
//     derivative = error - previousError;

//     // PID output
//     correction =
//         (kp * error) +
//         (ki * integral) +
//         (kd * derivative);

//     // Save current error for next iteration
//     previousError = error;

//     // Calculate motor speeds
//     int leftOutput = currentBaseSpeed + (int)correction;
//     int rightOutput = currentBaseSpeed - (int)correction;

//     // Constrain to valid PWM range
//     leftOutput = constrain(leftOutput, MIN_SPEED, MAX_SPEED);
//     rightOutput = constrain(rightOutput, MIN_SPEED, MAX_SPEED);

//     // Store the outputs; actual motor update happens in applyMotorOutput()
//     setMotor(leftOutput, rightOutput);
// }

// //==========================================================
// // DYNAMIC SPEED PROFILING
// //==========================================================

// void updateSpeedProfile()
// {
//     if (!lineDetected)
//     {
//         // Slow down while searching for the line
//         currentBaseSpeed = 110;
//         return;
//     }

//     float absError = fabs(error);

//     // High speed on straights
//     if (absError <= 0.20f)
//     {
//         currentBaseSpeed = 210;
//     }
//     // Medium speed on gentle curves
//     else if (absError <= 0.80f)
//     {
//         currentBaseSpeed = 185;
//     }
//     // Slower on moderate curves
//     else if (absError <= 1.50f)
//     {
//         currentBaseSpeed = 160;
//     }
//     // Slowest on sharp turns
//     else
//     {
//         currentBaseSpeed = 130;
//     }

//     currentBaseSpeed = constrain(currentBaseSpeed, MIN_SPEED, MAX_SPEED);
// }

// //==========================================================
// // LOST LINE RECOVERY
// //==========================================================

// void recoverLine()
// {
//     if (lineDetected)
//     {
//         lostStartTime = millis();
//         return;
//     }

//     uint32_t elapsed = millis() - lostStartTime;

//     // Stage 1: Quick correction
//     if (elapsed < 150)
//     {
//         if (lastSeenLeft)
//         {
//             setMotor(-140, 180);
//         }
//         else if (lastSeenRight)
//         {
//             setMotor(180, -140);
//         }
//         else
//         {
//             setMotor(-150, 150);
//         }

//         return;
//     }

//     // Stage 2: Aggressive pivot
//     if (elapsed < 350)
//     {
//         if (lastSeenLeft)
//         {
//             setMotor(-180, 200);
//         }
//         else if (lastSeenRight)
//         {
//             setMotor(200, -180);
//         }
//         else
//         {
//             setMotor(-180, 180);
//         }

//         return;
//     }

//     // Stage 3: Full search spin
//     if (elapsed < 700)
//     {
//         if (lastSeenLeft)
//         {
//             setMotor(-220, 220);
//         }
//         else if (lastSeenRight)
//         {
//             setMotor(220, -220);
//         }
//         else
//         {
//             setMotor(-220, 220);
//         }

//         return;
//     }

//     // Line not found within timeout
//     stopRobot();
// }

// //==========================================================
// // JUNCTION DETECTION
// //==========================================================

// void detectJunction()
// {
//     // A junction is detected when four or more sensors
//     // simultaneously see the line.
//     junctionDetected = (activeSensors >= 4);
// }

// //==========================================================
// // FINISH DETECTION
// //==========================================================

// void detectFinish()
// {
//     // Finish condition:
//     // All five sensors remain on black continuously
//     // for a short period to reject normal junctions.

//     if (activeSensors == SENSOR_COUNT)
//     {
//         if (finishTimer == 0)
//         {
//             finishTimer = millis();
//         }

//         if ((millis() - finishTimer) >= 300)
//         {
//             finishDetected = true;
//             raceFinished = true;
//             robotRunning = false;
//         }
//     }
//     else
//     {
//         finishTimer = 0;
//         finishDetected = false;
//     }
// }

// //==========================================================
// // RUNTIME PID TUNING
// // Serial Commands:
// //   KP=<value>
// //   KI=<value>
// //   KD=<value>
// //   BASE=<value>
// //   DEBUG=0
// //   DEBUG=1
// //==========================================================

// void runtimeTuning()
// {
//     if (!Serial.available())
//     {
//         return;
//     }

//     serialBuffer = Serial.readStringUntil('\n');
//     serialBuffer.trim();

//     if (serialBuffer.startsWith("KP="))
//     {
//         kp = serialBuffer.substring(3).toFloat();
//     }
//     else if (serialBuffer.startsWith("KI="))
//     {
//         ki = serialBuffer.substring(3).toFloat();
//     }
//     else if (serialBuffer.startsWith("KD="))
//     {
//         kd = serialBuffer.substring(3).toFloat();
//     }
//     else if (serialBuffer.startsWith("BASE="))
//     {
//         baseSpeed = constrain(
//             serialBuffer.substring(5).toInt(),
//             MIN_SPEED,
//             MAX_SPEED);
//     }
//     else if (serialBuffer == "DEBUG=1")
//     {
//         debugEnabled = true;
//     }
//     else if (serialBuffer == "DEBUG=0")
//     {
//         debugEnabled = false;
//     }
// }

// //==========================================================
// // DEBUG OUTPUT
// //==========================================================

// void debugOutput()
// {
//     if (!debugEnabled)
//     {
//         return;
//     }

//     if (millis() - debugTimer < 100)
//     {
//         return;
//     }

//     debugTimer = millis();

//     Serial.print("S:");

//     for (uint8_t i = 0; i < SENSOR_COUNT; i++)
//     {
//         Serial.print(sensorState[i]);
//         Serial.print(' ');
//     }

//     Serial.print(" Err=");
//     Serial.print(error, 2);

//     Serial.print(" PID=");
//     Serial.print(correction, 2);

//     Serial.print(" Base=");
//     Serial.print(currentBaseSpeed);

//     Serial.print(" L=");
//     Serial.print(leftMotorSpeed);

//     Serial.print(" R=");
//     Serial.print(rightMotorSpeed);

//     Serial.print(" J=");
//     Serial.print(junctionDetected);

//     Serial.print(" F=");
//     Serial.println(finishDetected);
// }

// //==========================================================
// // CENTRAL ROBOT UPDATE
// //==========================================================

// void updateRobot()
// {
//     if (!robotRunning || raceFinished)
//     {
//         stopRobot();
//         return;
//     }

//     readSensors();
//     calculatePosition();

//     detectJunction();
//     // detectFinish();

//     if (raceFinished)
//     {
//         stopRobot();
//         return;
//     }

//     runtimeTuning();

//     if (!lineDetected)
//     {
//         recoverLine();
//     }
//     else
//     {
//         updateSpeedProfile();
//         updatePID();
//     }

//     applyMotorOutput();
//     debugOutput();
// }

// //==========================================================
// // SETUP
// //==========================================================

// void setup()
// {
//     // Serial
//     Serial.begin(9600);

//     // Sensor pins
//     for (uint8_t i = 0; i < SENSOR_COUNT; i++)
//     {
//         pinMode(sensorPins[i], INPUT);
//     }

//     // Motor driver pins
//     pinMode(ENA, OUTPUT);
//     pinMode(ENB, OUTPUT);

//     pinMode(IN1, OUTPUT);
//     pinMode(IN2, OUTPUT);
//     pinMode(IN3, OUTPUT);
//     pinMode(IN4, OUTPUT);

//     // Safe startup state
//     stopRobot();

//     // Initialize timers
//     uint32_t now = millis();

//     lostStartTime = now;
//     debugTimer = now;
//     tuningTimer = now;
//     finishTimer = 0;

//     // Initialize runtime state
//     currentBaseSpeed = baseSpeed;
//     previousError = 0.0f;
//     integral = 0.0f;
//     derivative = 0.0f;
//     correction = 0.0f;

//     robotRunning = true;
//     raceFinished = false;

//     if (debugEnabled)
//     {
//         Serial.println(F("Competition Line Follower Ready"));
//     }
// }

// //==========================================================
// // LOOP
// //==========================================================

// void loop()
// {
//     updateRobot();
// }