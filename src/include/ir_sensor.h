#ifndef IR_SENSOR_H
#define IR_SENSOR_H

#include <Arduino.h>

#define NUM_SENSORS 10

void irSensorInit();
void irSensorBegin(const int pins[NUM_SENSORS]);
void irSensorReadAll();
int irSensorGetValue(int index);
void irSensorPrintValues();
String irSensorGetValuesAsString();

#endif