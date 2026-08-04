#include <Arduino.h>

// =================================================
// IR Sensor Arrays — 10 sensors total (two 5-sensor arrays combined)
// Array 1 (left half) -> Array 2 (right half), left to right overall
// =================================================
#define NUM_SENSORS 10

const uint8_t sensorPins[NUM_SENSORS] = {
    A0, A1, A2, A3, A4, // array 1 (left)
    A5, 2, 3, 4, 5      // array 2 (right)
};

// Odd, symmetric weights: no sensor sits exactly at zero, so a
// line centered between two middle sensors still produces a clean 0.
const int weights[NUM_SENSORS] = {-9, -7, -5, -3, -1, 1, 3, 5, 7, 9};

// true = sensor pulls LOW when it sees the dark line
#define LINE_IS_LOW true

bool sensor[NUM_SENSORS];
uint8_t sensorCount = 0;

// ------------------------------------------------------------
// MOTOR PINS -- PWM moved to 9 and 10 so BOTH motors share
// Timer1. Previously PWMA was on 10 (Timer1) and PWMB on 11
// (Timer2), which meant the two motors ran on separate timers.
// ------------------------------------------------------------
#define MOTOR_STBY 6
#define MOTOR_AIN1 7
#define MOTOR_AIN2 8
#define MOTOR_PWMA 9  // was 10
#define MOTOR_PWMB 10 // was 11
#define MOTOR_BIN1 11 // was 12
#define MOTOR_BIN2 12 // was 13

// =================================================
// PID SETTINGS
// NOTE: weights now range -9..9 (10 sensors) instead of -2..2 (5
// sensors), so Kp/Kd are scaled down from the 5-sensor version.
// Re-tune on your actual track.
// =================================================
const float Kp = 20;
const float Ki = 0;
const float Kd = 3;

float previousError = 0;
float totalError = 0;

// =================================================
// SPEED
// =================================================
int normalSpeed = 90;
int slowSpeed = 60;
int currentSpeed = 90;

const float turnInnerRatio = 0.1;
const float turnOuterRatio = .8;

// =================================================
// Motor Control
// =================================================
void setMotor(int left, int right)
{
    left = constrain(left, -255, 255);
    right = constrain(right, -255, 255);

    // Left motor (Motor A)
    if (left >= 0)
    {
        digitalWrite(MOTOR_AIN1, HIGH);
        digitalWrite(MOTOR_AIN2, LOW);
        analogWrite(MOTOR_PWMA, left);
    }
    else
    {
        digitalWrite(MOTOR_AIN1, LOW);
        digitalWrite(MOTOR_AIN2, HIGH);
        analogWrite(MOTOR_PWMA, -left);
    }

    // Right motor (Motor B)
    if (right >= 0)
    {
        digitalWrite(MOTOR_BIN1, HIGH);
        digitalWrite(MOTOR_BIN2, LOW);
        analogWrite(MOTOR_PWMB, right);
    }
    else
    {
        digitalWrite(MOTOR_BIN1, LOW);
        digitalWrite(MOTOR_BIN2, HIGH);
        analogWrite(MOTOR_PWMB, -right);
    }
}

// =================================================
// Read Sensors
// =================================================
void readSensors()
{
    sensorCount = 0;
    for (int i = 0; i < NUM_SENSORS; i++)
    {
        bool lineDetected = LINE_IS_LOW
                                ? (digitalRead(sensorPins[i]) == LOW)
                                : (digitalRead(sensorPins[i]) == HIGH);
        sensor[i] = lineDetected;
        if (lineDetected)
            sensorCount++;
    }
}

// =================================================
// Calculate Error
// =================================================
float getError()
{
    readSensors();

    int sumOfError = 0;

    for (int i = 0; i < NUM_SENSORS; i++)
    {
        if (sensor[i])
        {
            sumOfError += weights[i];
        }
    }

    // Line lost
    if (sensorCount == 0)
    {
        totalError = 0;

        if (previousError < 0)
            return -9;

        if (previousError > 0)
            return 9;

        return 0;
    }

    return (float)sumOfError / sensorCount;
}

// =================================================
// Brake
// =================================================
void breakCar()
{
    // setMotor(-currentSpeed, -currentSpeed);
    // delay(50);
    // setMotor(0, 0);
    // delay(50);
}

// =================================================
// 90 Degree Left Turn
// With 10 sensors, "back to centered" is checked on the two middle
// sensors (indices 4 and 5) instead of a single center sensor.
// =================================================
void turnLeft()
{
    int innerSpeed = (int)(currentSpeed * turnInnerRatio);
    int outerSpeed = (int)(currentSpeed * turnOuterRatio);

    setMotor(innerSpeed, outerSpeed);

    while (true)
    {
        readSensors();

        if (sensor[4] || sensor[5])
            break;
    }

    delay(50);
}

// =================================================
// 90 Degree Right Turn
// =================================================
void turnRight()
{
    int innerSpeed = (int)(currentSpeed * turnInnerRatio);
    int outerSpeed = (int)(currentSpeed * turnOuterRatio);

    setMotor(outerSpeed, innerSpeed);

    while (true)
    {
        readSensors();

        if (sensor[4] || sensor[5])
            break;
    }

    delay(50);
}

// =================================================
// Setup
// =================================================
void setup()
{
    Serial.begin(115200);

    for (int i = 0; i < NUM_SENSORS; i++)
        pinMode(sensorPins[i], INPUT);

    pinMode(MOTOR_STBY, OUTPUT);
    pinMode(MOTOR_AIN1, OUTPUT);
    pinMode(MOTOR_AIN2, OUTPUT);
    pinMode(MOTOR_PWMA, OUTPUT);
    pinMode(MOTOR_BIN1, OUTPUT);
    pinMode(MOTOR_BIN2, OUTPUT);
    pinMode(MOTOR_PWMB, OUTPUT);

    digitalWrite(MOTOR_STBY, HIGH);

    Serial.println("Robot Ready (10-sensor)");
}

// =================================================
// Main Loop
// =================================================
void loop()
{
    float error = getError();

    for (int i = 0; i < NUM_SENSORS; i++)
    {
        Serial.print(" ");
        Serial.print(sensor[i]);
    }
    Serial.println("");

    // ----------------------------
    // Corner Detection (debounced)
    // Outermost pair on each side (indices 0/1 and 8/9)
    // ----------------------------
    static int leftCornerCount = 0;
    static int rightCornerCount = 0;
    const int cornerConfirmCount = 3;

    if (sensor[0] && sensor[1])
    {
        leftCornerCount++;
        rightCornerCount = 0;
    }
    else if (sensor[8] && sensor[9])
    {
        rightCornerCount++;
        leftCornerCount = 0;
    }
    else
    {
        leftCornerCount = 0;
        rightCornerCount = 0;
    }

    if (leftCornerCount >= cornerConfirmCount)
    {
        leftCornerCount = 0;
        rightCornerCount = 0;
        breakCar();
        turnLeft();
        return;
    }

    if (rightCornerCount >= cornerConfirmCount)
    {
        leftCornerCount = 0;
        rightCornerCount = 0;
        breakCar();
        turnRight();
        return;
    }

    // ----------------------------
    // PID
    // ----------------------------

    totalError += error;
    totalError = constrain(totalError, -20, 20);

    float derivative = error - previousError;

    float correction =
        (Kp * error) +
        (Ki * totalError) +
        (Kd * derivative);

    previousError = error;

    correction = constrain(correction, -70, 70);

    int speed = normalSpeed;

    if (abs(error) > 4)
        speed = slowSpeed;

    currentSpeed = speed;

    int leftMotor = speed - correction;
    int rightMotor = speed + correction;

    setMotor(leftMotor, rightMotor);

    // ----------------------------
    // Debug
    // ----------------------------

    Serial.print(" Error:");
    Serial.print(error);

    Serial.print(" Corr:");
    Serial.print(correction);

    Serial.print(" L:");
    Serial.print(leftMotor);

    Serial.print(" R:");
    Serial.println(rightMotor);

    delay(10);
}