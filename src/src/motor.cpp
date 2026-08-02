#include "motor.h"
#include "config.h"

static int _speedA = 0;
static int _speedB = 0;
static int _dirA = 0; // 1 = forward, -1 = backward, 0 = stopped
static int _dirB = 0;

void motorBegin()
{
    pinMode(MOTOR_STBY, OUTPUT);
    pinMode(MOTOR_AIN1, OUTPUT);
    pinMode(MOTOR_AIN2, OUTPUT);
    pinMode(MOTOR_PWMA, OUTPUT);
    pinMode(MOTOR_BIN1, OUTPUT);
    pinMode(MOTOR_BIN2, OUTPUT);
    pinMode(MOTOR_PWMB, OUTPUT);

    motorStandby(true);
    motorStopAll();
}

void motorStandby(bool enable)
{
    digitalWrite(MOTOR_STBY, enable ? HIGH : LOW);
}

void motorAForward(int speed)
{
    speed = constrain(speed, 0, 255);
    digitalWrite(MOTOR_AIN1, HIGH);
    digitalWrite(MOTOR_AIN2, LOW);
    analogWrite(MOTOR_PWMA, speed);
    _speedA = speed;
    _dirA = 1;
}

void motorABackward(int speed)
{
    speed = constrain(speed, 0, 255);
    digitalWrite(MOTOR_AIN1, LOW);
    digitalWrite(MOTOR_AIN2, HIGH);
    analogWrite(MOTOR_PWMA, speed);
    _speedA = speed;
    _dirA = -1;
}

void motorAStop()
{
    digitalWrite(MOTOR_AIN1, LOW);
    digitalWrite(MOTOR_AIN2, LOW);
    analogWrite(MOTOR_PWMA, 0);
    _speedA = 0;
    _dirA = 0;
}

void motorBForward(int speed)
{
    speed = constrain(speed, 0, 255);
    digitalWrite(MOTOR_BIN1, HIGH);
    digitalWrite(MOTOR_BIN2, LOW);
    analogWrite(MOTOR_PWMB, speed);
    _speedB = speed;
    _dirB = 1;
}

void motorBBackward(int speed)
{
    speed = constrain(speed, 0, 255);
    digitalWrite(MOTOR_BIN1, LOW);
    digitalWrite(MOTOR_BIN2, HIGH);
    analogWrite(MOTOR_PWMB, speed);
    _speedB = speed;
    _dirB = -1;
}

void motorBStop()
{
    digitalWrite(MOTOR_BIN1, LOW);
    digitalWrite(MOTOR_BIN2, LOW);
    analogWrite(MOTOR_PWMB, 0);
    _speedB = 0;
    _dirB = 0;
}

void motorStopAll()
{
    motorAStop();
    motorBStop();
}

bool motorAIsRunning() { return _dirA != 0; }
bool motorBIsRunning() { return _dirB != 0; }
int motorAGetSpeed() { return _speedA; }
int motorBGetSpeed() { return _speedB; }

String motorAGetDirection()
{
    if (_dirA == 1)
        return "FORWARD";
    if (_dirA == -1)
        return "BACKWARD";
    return "STOPPED";
}

String motorBGetDirection()
{
    if (_dirB == 1)
        return "FORWARD";
    if (_dirB == -1)
        return "BACKWARD";
    return "STOPPED";
}