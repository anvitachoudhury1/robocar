#include <Servo.h>

#include <Arduino.h>
#include "door_control.h"
#include "config.h"
Servo doorServo;
static bool doorState = false;
void doorInit()
{
    doorServo.attach(SERVO_PIN);
    doorServo.write(0); // start closed
}

void doorOpen()
{
    doorServo.write(90);
    doorState = true;
}
void doorClose()
{
    doorServo.write(0);
    doorState = false;
}

bool doorcontrol()
{
    return doorState;
}