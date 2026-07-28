#include <Arduino.h>

// =================================================
// RLS-05 Sensor Pins
// IR1 LEFT -> IR5 RIGHT
// =================================================
const byte sensorPins[5] = {4, 5, A1, 7, 8};
const int weights[5] = {-2, -1, 0, 1, 2};

bool sensors[5];

// =================================================
// L298N Motor Pins
// =================================================
const byte ENA = 6; // Left motor PWM
const byte IN1 = 11;
const byte IN2 = 12;

const byte ENB = 3; // Right motor PWM
const byte IN3 = 13;
const byte IN4 = 10;

// =================================================
// PID SETTINGS
// =================================================
float Kp = 22;
float Ki = 0;
float Kd = 10;

float previousError = 0;
float integral = 0;

// =================================================
// SPEED
// =================================================
int normalSpeed = 90;
int slowSpeed = 60;

// =================================================
// Motor Control
// =================================================
void setMotor(int left, int right)
{
    left = constrain(left, -255, 255);
    right = constrain(right, -255, 255);

    // Left motor
    if (left >= 0)
    {
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        analogWrite(ENA, left);
    }
    else
    {
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        analogWrite(ENA, -left);
    }

    // Right motor
    if (right >= 0)
    {
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
        analogWrite(ENB, right);
    }
    else
    {
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
        analogWrite(ENB, -right);
    }
}

// =================================================
// Read Sensors
// =================================================
void readSensors()
{
    for (int i = 0; i < 5; i++)
    {
        sensors[i] = (digitalRead(sensorPins[i]) == LOW);
    }
}

// =================================================
// Calculate Error
// =================================================
float getError()
{
    readSensors();

    int sum = 0;
    int count = 0;

    for (int i = 0; i < 5; i++)
    {
        if (sensors[i])
        {
            sum += weights[i];
            count++;
        }
    }

    // Line lost
    if (count == 0)
    {
        integral = 0;

        if (previousError < 0)
            return -3;

        if (previousError > 0)
            return 3;

        return 0;
    }

    return (float)sum / count;
}

// =================================================
// 90 Degree Left Turn
// =================================================
void turnLeft()
{
    setMotor(-60, 120);

    while (true)
    {
        readSensors();

        if (sensors[2])
            break;
    }

    delay(50);
}

// =================================================
// 90 Degree Right Turn
// =================================================
void turnRight()
{
    setMotor(120, -60);

    while (true)
    {
        readSensors();

        if (sensors[2])
            break;
    }

    delay(50);
}

// =================================================
// Setup
// =================================================
void setup()
{
    Serial.begin(9600);

    for (int i = 0; i < 5; i++)
        pinMode(sensorPins[i], INPUT);

    pinMode(ENA, OUTPUT);
    pinMode(ENB, OUTPUT);

    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);

    Serial.println("Robot Ready");
}

// =================================================
// Main Loop
// =================================================
void loop()
{

    float error = getError();

    // ----------------------------
    // Corner Detection
    // ----------------------------

    // Left corner
    if (sensors[0] && sensors[1])
    {
        turnLeft();
        return;
    }

    // Right corner
    if (sensors[3] && sensors[4])
    {
        turnRight();
        return;
    }

    // ----------------------------
    // PID
    // ----------------------------

    integral += error;
    integral = constrain(integral, -20, 20);

    float derivative = error - previousError;

    float correction =
        (Kp * error) +
        (Ki * integral) +
        (Kd * derivative);

    previousError = error;

    correction = constrain(correction, -70, 70);

    int speed = normalSpeed;

    if (abs(error) > 1.5)
        speed = slowSpeed;

    // IMPORTANT:
    // Direction corrected
    int leftMotor = speed - correction;
    int rightMotor = speed + correction;

    setMotor(leftMotor, rightMotor);

    // ----------------------------
    // Debug
    // ----------------------------
    for (int i = 0; i < 5; i++)
    {
        Serial.print(sensors[i]);
        Serial.print(" ");
    }

    Serial.print(" Error:");
    Serial.print(error);

    Serial.print(" Corr:");
    Serial.print(correction);

    Serial.print(" L:");
    Serial.print(leftMotor);

    Serial.print(" R:");
    Serial.println(rightMotor);

    delay(15);
}