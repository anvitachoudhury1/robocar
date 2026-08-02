#include <Arduino.h>

// =================================================
// IR Sensor Array (Array 1) — analog pins used as digital
// S1 (left) -> S5 (right)
// =================================================
// #define IR_A1_S1 A0
// #define IR_A1_S2 A1
// #define IR_A1_S3 A2
// #define IR_A1_S4 A3
// #define IR_A1_S5 A4

#define IR_A2_S1 A5
#define IR_A2_S2 2
#define IR_A2_S3 3
#define IR_A2_S4 4
#define IR_A2_S5 5

const byte sensorPins[5] = {IR_A2_S1, IR_A2_S2, IR_A2_S3, IR_A2_S4, IR_A2_S5};
const int weights[5] = {-2, -1, 0, 1, 2};

bool sensors[5];

// =================================================
// TB6612FNG Motor Driver Pins
// Motor A = left, Motor B = right
// =================================================
#define MOTOR_STBY 6  // standby -- must be HIGH to enable driver
#define MOTOR_AIN1 7  // motor A direction pin 1
#define MOTOR_AIN2 8  // motor A direction pin 2
#define MOTOR_PWMA 10 // motor A speed (PWM)
#define MOTOR_BIN1 12 // motor B direction pin 1
#define MOTOR_BIN2 13 // motor B direction pin 2
#define MOTOR_PWMB 11 // motor B speed (PWM)

// =================================================
// PID SETTINGS
// =================================================
float Kp = 16;
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
    Serial.begin(115200);

    for (int i = 0; i < 5; i++)
        pinMode(sensorPins[i], INPUT);

    pinMode(MOTOR_STBY, OUTPUT);
    pinMode(MOTOR_AIN1, OUTPUT);
    pinMode(MOTOR_AIN2, OUTPUT);
    pinMode(MOTOR_PWMA, OUTPUT);
    pinMode(MOTOR_BIN1, OUTPUT);
    pinMode(MOTOR_BIN2, OUTPUT);
    pinMode(MOTOR_PWMB, OUTPUT);

    digitalWrite(MOTOR_STBY, HIGH); // enable the driver -- required for TB6612FNG

    Serial.println("Robot Ready");
}

// =================================================
// Main Loop
// =================================================
void loop()
{

    // setMotor(100, 100);
    // delay(100);
    // return;
    float error = getError();

    for (int i = 0; i < 5; i++)
    {
        // Serial.print(" S");
        // Serial.print(i);
        Serial.print(" ");
        Serial.print(sensors[i]);
    }
    Serial.println("");

    // return;
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

    delay(15);
}