#include <Arduino.h>

#define ENA 5
#define IN1 6
#define IN2 7

void setup()
{
    Serial.begin(9600);
    pinMode(ENA, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);

    // Set direction: forward
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
}

void loop()
{
    // Ramp up from slow to fast
    for (int speed = 0; speed <= 255; speed += 5)
    {
        analogWrite(ENA, speed);
        Serial.print("Speed: ");
        Serial.println(speed);
        delay(100);
    }

    delay(1000); // stay at full speed briefly

    // Ramp back down from fast to slow
    for (int speed = 255; speed >= 0; speed -= 5)
    {
        analogWrite(ENA, speed);
        Serial.print("Speed: ");
        Serial.println(speed);
        delay(100);
    }

    delay(1000); // pause at stop before repeating
}