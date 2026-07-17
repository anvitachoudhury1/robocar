#ifndef DOOR_CONTROL_H
#define DOOR_CONTROL_H

#include <Arduino.h>

void doorInit();

void doorOpen();
void doorClose();

bool doorcontrol();
#endif // DOOR_CONTROL_H