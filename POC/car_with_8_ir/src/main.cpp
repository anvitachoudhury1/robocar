#include <Arduino.h>

// =================================================
// IR Sensor Array — 8 sensors, analog inputs A0..A7, left to right
// =================================================
#define NUM_SENSORS 8

const uint8_t sensorPins[NUM_SENSORS] = {A0, A1, A2, A3, A4, A5, A6, A7};

// Anything past this counts as "on line". Adjust after you see your
// actual numbers.
const uint16_t THRESHOLD = 800;

// Debounce band around THRESHOLD: an "on" sensor only turns off below
// THRESHOLD-HYSTERESIS, an "off" sensor only turns on above
// THRESHOLD+HYSTERESIS. Stops single-ADC-sample flicker near the
// threshold from toggling a sensor bit every loop tick.
const uint16_t HYSTERESIS = 40;

// Average this many quick back-to-back ADC reads per sensor per tick.
// Filters transient ADC noise at the source instead of on the derived
// error signal, so it doesn't add lag to the control loop.
const uint8_t SENSOR_SAMPLES = 3;

// Odd, symmetric weights: no sensor sits exactly at zero, so a
// line centered between two middle sensors still produces a clean 0.
const int weights[NUM_SENSORS] = {-7, -5, -3, -1, 1, 3, 5, 7};

bool sensor[NUM_SENSORS];
uint8_t sensorCount = 0;

// ------------------------------------------------------------
// MOTOR PINS -- PWM on 9 and 10 so BOTH motors share
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
// weights range -7..7 (8 sensors). Re-tune on your actual track.
// =================================================
const float Kp = 20;
const float Ki = 0;
const float Kd = 3;

float previousError = 0;
float totalError = 0;

// =================================================
// SPEED
// =================================================
int normalSpeed = 120;
int slowSpeed = 60;
int currentSpeed = 60;

// Negative inner ratio = inner wheel spins in reverse, so the robot
// pivots close to in-place instead of arcing forward on both wheels.
// A forward-only inner wheel has a turning radius bounded by the
// wheelbase and can't tighten up enough for a small/sharp or U-turn.
const float turnInnerRatio = -0.3;
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
// Noise Filter
// A real line under the array lights up a contiguous run of
// sensors. Crosstalk/reflection noise instead shows up as an
// isolated sensor past THRESHOLD, split off from the real block by
// a gap of sensors that are NOT past THRESHOLD. So: find the
// largest contiguous "on" run anywhere in the array (not just one
// anchored on the center sensors -- the line can drift fully to one
// side, e.g. mid-turn, and still be real) and treat anything past
// THRESHOLD outside that run as noise.
// =================================================
void filterSensorNoise()
{
    int bestStart = -1, bestLen = 0;
    int curStart = -1, curLen = 0;

    for (int i = 0; i < NUM_SENSORS; i++)
    {
        if (sensor[i])
        {
            if (curStart == -1)
                curStart = i;
            curLen++;
        }
        else
        {
            if (curLen > bestLen)
            {
                bestLen = curLen;
                bestStart = curStart;
            }
            curStart = -1;
            curLen = 0;
        }
    }
    if (curLen > bestLen)
    {
        bestLen = curLen;
        bestStart = curStart;
    }

    if (bestLen == 0)
        return;

    for (int i = 0; i < NUM_SENSORS; i++)
    {
        if (i < bestStart || i >= bestStart + bestLen)
            sensor[i] = false;
    }
}

// =================================================
// Read Sensors
// =================================================
void readSensors()
{
    for (uint8_t i = 0; i < NUM_SENSORS; i++)
    {
        uint16_t total = 0;
        for (uint8_t s = 0; s < SENSOR_SAMPLES; s++)
            total += analogRead(sensorPins[i]);
        uint16_t value = total / SENSOR_SAMPLES;

        uint16_t cutoff = sensor[i] ? THRESHOLD - HYSTERESIS : THRESHOLD + HYSTERESIS;
        sensor[i] = value > cutoff;
    }

    filterSensorNoise();

    sensorCount = 0;
    for (uint8_t i = 0; i < NUM_SENSORS; i++)
    {
        if (sensor[i])
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
            return -7;

        if (previousError > 0)
            return 7;

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
// With 8 sensors, "back to centered" is checked on the two middle
// sensors (indices 3 and 4) instead of a single center sensor.
// =================================================
void turnLeft()
{
    int innerSpeed = (int)(currentSpeed * turnInnerRatio);
    int outerSpeed = (int)(currentSpeed * turnOuterRatio);

    setMotor(innerSpeed, outerSpeed);

    while (true)
    {
        readSensors();

        if (sensor[3] || sensor[4])
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

        if (sensor[3] || sensor[4])
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

    pinMode(MOTOR_STBY, OUTPUT);
    pinMode(MOTOR_AIN1, OUTPUT);
    pinMode(MOTOR_AIN2, OUTPUT);
    pinMode(MOTOR_PWMA, OUTPUT);
    pinMode(MOTOR_BIN1, OUTPUT);
    pinMode(MOTOR_BIN2, OUTPUT);
    pinMode(MOTOR_PWMB, OUTPUT);

    digitalWrite(MOTOR_STBY, HIGH);

    Serial.println("Robot Ready (8-sensor)");
}

// =================================================
// Main Loop
// =================================================
void loop()
{
    float error = getError();

    // ----------------------------
    // Corner Detection (debounced)
    // Outermost pair on each side (indices 0/1 and 6/7)
    // ----------------------------
    static int leftCornerCount = 0;
    static int rightCornerCount = 0;
    const int cornerConfirmCount = 2;

    if (sensor[0] && sensor[1])
    {
        leftCornerCount++;
        rightCornerCount = 0;
    }
    else if (sensor[6] && sensor[7])
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

    int leftMotor = speed + correction;
    int rightMotor = speed - correction;

    setMotor(leftMotor, rightMotor);

    // ----------------------------
    // Debug
    // ----------------------------

    Serial.print(F("  ["));
    for (uint8_t i = 0; i < NUM_SENSORS; i++)
    {
        Serial.print(sensor[i] ? '#' : '.');
    }
    Serial.print("][");
    Serial.print(leftMotor);
    Serial.print(":");
    Serial.print(rightMotor);
    Serial.println(']');

    // Serial.print(" Error:");
    // Serial.print(error);

    // Serial.print(" Corr:");
    // Serial.print(correction);

    // Serial.print(" L:");
    // Serial.print(leftMotor);

    // Serial.print(" R:");
    // Serial.println(rightMotor);

    delay(10);
}
