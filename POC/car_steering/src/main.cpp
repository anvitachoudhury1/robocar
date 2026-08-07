#include <Arduino.h>

// =================================================
// IR Sensor Array (Array 2) — analog pin + digital pins
// S1 (left) -> S5 (right)
// =================================================
#define IR_A2_S1 A5
#define IR_A2_S2 A4
#define IR_A2_S3 A3
#define IR_A2_S4 A2
#define IR_A2_S5 A1

const byte sensorPins[5] = {IR_A2_S1, IR_A2_S2, IR_A2_S3, IR_A2_S4, IR_A2_S5};
const int weights[5] = {-2, -1, 0, 1, 2};

bool sensors[5];

// =================================================
// TB6612FNG Motor Driver Pins
// Motor A = left, Motor B = right
// =================================================
#define MOTOR_STBY 6
#define MOTOR_AIN1 7
#define MOTOR_AIN2 8
#define MOTOR_PWMA 10
#define MOTOR_BIN1 12
#define MOTOR_BIN2 13
#define MOTOR_PWMB 11

// =================================================
// PID SETTINGS
// =================================================
const float Kp = 16;
const float Ki = 0;
const float Kd = 10;

float previousError = 0;
float totalError = 0;

// =================================================
// SPEED
// =================================================
int normalSpeed = 90;
int slowSpeed = 60;

// Tracks whatever speed the car was actually driving at, moment to
// moment (normalSpeed or slowSpeed depending on how far off-line it
// currently is). Turn speeds are based on THIS, not a fixed constant,
// so a turn taken while already slowing down for a sharp curve stays
// proportionally scaled to that slower speed, not full normalSpeed.
int currentSpeed = 90;

// Turn speeds are derived from normalSpeed (not hardcoded) so they
// scale automatically if you change normalSpeed later.
//   turnInnerRatio: the inner wheel during a pivot turn -- negative
//     means it briefly reverses, for a tighter pivot.
//   turnOuterRatio: the outer wheel -- boosted above normalSpeed so
//     the turn completes quickly.
const float turnInnerRatio = 0.3; // e.g. normalSpeed=90 -> 27 (slows, doesn't reverse -- wider/gentler turn)
const float turnOuterRatio = 1.0; // e.g. normalSpeed=90 -> 90 (stays near normal, doesn't speed up)

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

    int sumOfError = 0;
    int sensorsActiveCount = 0;

    for (int i = 0; i < 5; i++)
    {
        if (sensors[i])
        {
            sumOfError += weights[i];
            sensorsActiveCount++;
        }
    }

    // Line lost
    if (sensorsActiveCount == 0)
    {
        totalError = 0;

        if (previousError < 0)
            return -3;

        if (previousError > 0)
            return 3;

        return 0;
    }

    return (float)sumOfError / sensorsActiveCount;
}

// =================================================
// Brake — brief reverse pulse for a true stop, used right before
// committing to a turn so the car doesn't carry momentum into the pivot
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
// =================================================
void turnLeft()
{
    int innerSpeed = (int)(currentSpeed * turnInnerRatio);
    int outerSpeed = (int)(currentSpeed * turnOuterRatio);

    setMotor(innerSpeed, outerSpeed);

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
    int innerSpeed = (int)(currentSpeed * turnInnerRatio);
    int outerSpeed = (int)(currentSpeed * turnOuterRatio);

    setMotor(outerSpeed, innerSpeed);

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

    digitalWrite(MOTOR_STBY, HIGH);

    Serial.println("Robot Ready");
}

// =================================================
// Main Loop
// =================================================
void loop()
{
    // setMotor(60, 60);
    // return;
    float error = getError();

    // ----------------------------
    // Corner Detection
    // ----------------------------

    // ----------------------------
    // Corner Detection (debounced)
    // ----------------------------
    // Require the same corner pattern to hold for several consecutive
    // loop cycles before committing to a turn. A single noisy/flickered
    // sensor reading can otherwise trigger the wrong-direction turn.
    static int leftCornerCount = 0;
    static int rightCornerCount = 0;
    const int cornerConfirmCount = 3; // ~3 * 15ms = 45ms of sustained detection

    if (sensors[0] && sensors[1])
    {
        leftCornerCount++;
        rightCornerCount = 0;
    }
    else if (sensors[3] && sensors[4])
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

    if (abs(error) > 1.5)
        speed = slowSpeed;

    currentSpeed = speed; // remember the speed actually being driven right now

    int leftMotor = speed - correction;
    int rightMotor = speed + correction;

    setMotor(leftMotor, rightMotor);

    // ----------------------------
    // Debug
    // ----------------------------

    for (int i = 0; i < 5; i++)
    {
        Serial.print(" ");
        Serial.print(sensors[i]);
    }

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