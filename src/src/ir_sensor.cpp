#include "ir_sensor.h"
#include "config.h"

static int _pins[NUM_SENSORS];
static int _values[NUM_SENSORS];
static int irPins[NUM_SENSORS] = {
    IR_A1_S1, IR_A1_S2, IR_A1_S3, IR_A1_S4, IR_A1_S5,
    IR_A2_S1, IR_A2_S2, IR_A2_S3, IR_A2_S4, IR_A2_S5};

void irSensorInit()
{
    irSensorBegin(irPins);
}
void irSensorBegin(const int pins[NUM_SENSORS])
{
    for (int i = 0; i < NUM_SENSORS; i++)
    {
        _pins[i] = pins[i];
        pinMode(_pins[i], INPUT);
    }
}

void irSensorReadAll()
{
    for (int i = 0; i < NUM_SENSORS; i++)
    {
        _values[i] = digitalRead(_pins[i]);
    }
}

int irSensorGetValue(int index)
{
    if (index < 0 || index >= NUM_SENSORS)
        return -1;
    return _values[index];
}

void irSensorPrintValues()
{
    Serial.print("Array1: ");
    for (int i = 0; i < 5; i++)
    {
        Serial.print(_values[i]);
        Serial.print(" ");
    }

    Serial.print(" | Array2: ");
    for (int i = 5; i < NUM_SENSORS; i++)
    {
        Serial.print(_values[i]);
        Serial.print(" ");
    }

    Serial.println();
}

String irSensorGetValuesAsString()
{
    String result = "Array1: ";
    for (int i = 0; i < 5; i++)
    {
        result += String(_values[i]);
        result += " ";
    }

    result += "| Array2: ";
    for (int i = 5; i < NUM_SENSORS; i++)
    {
        result += String(_values[i]);
        result += " ";
    }

    return result;
}
