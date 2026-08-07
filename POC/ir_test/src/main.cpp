//
#include <Arduino.h>

// =====================================================
// Arduino UNO R4 WiFi
// 5-Sensor IR Array Test
//
// Sensor order:
// S1 (Left) -----> S5 (Right)
//
// Your sensor logic:
// HIGH = BLACK
// LOW  = WHITE
// =====================================================
// A1 A4
const uint8_t sensorPins[5] = {A4, A3, A2, A1};

void setup()
{
    Serial.begin(115200);
    delay(2000);

    for (int i = 0; i < 5; i++)
    {
        pinMode(sensorPins[i], INPUT);
    }

    Serial.println();
    Serial.println("====================================");
    Serial.println("5 Sensor IR Array Test");
    Serial.println("B = Black   W = White");
    Serial.println("====================================");
}

void loop()
{
    for (int i = 0; i < 5; i++)
    {
        if (digitalRead(sensorPins[i]) == HIGH)
        {
            Serial.print("B ");
        }
        else
        {
            Serial.print("W ");
        }
    }

    Serial.println();

    delay(100);
}