#include "servo_control.h"
#include "config.h"
#include <Servo.h>

static Servo _servo;
static int _currentAngle = 0;

void servoInit()
{
    _servo.attach(SERVO_PIN);
    servoSetAngle(90); // default starting position
}

void servoSetAngle(int angle)
{
    angle = constrain(angle, 0, 180);
    _servo.write(angle);
    _currentAngle = angle;
}

int servoGetAngle()
{
    return _currentAngle;
}

void servoSweep(int fromAngle, int toAngle, int stepDelayMs)
{
    fromAngle = constrain(fromAngle, 0, 180);
    toAngle = constrain(toAngle, 0, 180);

    if (fromAngle <= toAngle)
    {
        for (int a = fromAngle; a <= toAngle; a++)
        {
            servoSetAngle(a);
            delay(stepDelayMs);
        }
    }
    else
    {
        for (int a = fromAngle; a >= toAngle; a--)
        {
            servoSetAngle(a);
            delay(stepDelayMs);
        }
    }
}