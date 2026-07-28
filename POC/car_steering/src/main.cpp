#include <Arduino.h>

// =======================================================
//                SENSOR PINS
// =======================================================
// S0(left)=4, S1=5, S2(center)=A1, S3=7, S4(right)=8

const int sensorPins[5] = {4, 5, A1, 7, 8};

// Position weights
const int weights[5] = {-2, -1, 0, 1, 2};

// =======================================================
//                MOTOR DRIVER (L298N)
// =======================================================

const int ENA = 6;
const int IN1 = 11;
const int IN2 = 12;

const int ENB = A2; // Digital only
const int IN3 = 13;
const int IN4 = 10;

// =======================================================
//                   PID
// =======================================================

float Kp = 18.0;
float Ki = 0.0;
float Kd = 10.0;

float error = 0;
float previousError = 0;
float integral = 0;
float derivative = 0;
float correction = 0;

// =======================================================
//                MOTOR SPEED
// =======================================================

int motorSpeed = 180;

// =======================================================
//          READ LINE POSITION
// =======================================================

float readLinePosition()
{
    int activeSensors = 0;
    int weightedSum = 0;

    for (int i = 0; i < 5; i++)
    {
        int sensorValue = digitalRead(sensorPins[i]);

        // LOW = Black line
        if (sensorValue == LOW)
        {
            weightedSum += weights[i];
            activeSensors++;
        }
    }

    // Lost line
    if (activeSensors == 0)
    {
        return previousError;
    }

    return (float)weightedSum / activeSensors;
}

// =======================================================
//                 MOTOR CONTROL
// =======================================================

void moveForward(int speedValue)
{
    // Left Motor
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);

    // Right Motor
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);

    analogWrite(ENA, speedValue);

    // A2 is not PWM
    digitalWrite(ENB, HIGH);
}

// =======================================================
//                PID CONTROLLER
// =======================================================

void lineFollowPID()
{
    error = readLinePosition();

    integral += error;

    derivative = error - previousError;

    correction =
        (Kp * error) +
        (Ki * integral) +
        (Kd * derivative);

    previousError = error;

    Serial.print("Error: ");
    Serial.print(error);
}
// =======================================================
//                    SETUP
// =======================================================

void setup()
{
    Serial.begin(115200);

    for (int i = 0; i < 5; i++)
    {
        pinMode(sensorPins[i], INPUT);
    }

    pinMode(ENA, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);

    pinMode(ENB, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);

    delay(1000);
}

// =======================================================
//                     LOOP
// =======================================================

void loop()
{
    moveForward(motorSpeed);

    lineFollowPID();

    delay(5);
}