#include <Arduino.h>
#define TRIG_PIN 11
#define ECHO_PIN 10

void setup()
{
    Serial.begin(9600);
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
}

void loop()
{
    // Send trigger pulse
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    // Measure echo time (30ms timeout so it never hangs)
    long duration = pulseIn(ECHO_PIN, HIGH, 30000);

    // Convert to distance in cm
    float distanceCm = (duration * 0.0343) / 2;

    Serial.print("Distance: ");
    Serial.print(distanceCm);
    Serial.println(" cm");

    delay(300);
}