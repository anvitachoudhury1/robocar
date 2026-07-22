// /*
//   Servo Isolation Test
//   ONLY the servo — no motor, no sensors, no other code.
//   Just moves the servo to 90, waits, then sweeps a bit, so you can
//   confirm the servo itself + wiring + power is working correctly
//   before adding anything else back in.
// */

// #include <Arduino.h>
// #include <Servo.h>

// const int servoPin = 3;
// Servo myServo;

// const int ENA = 6;   // Left motor speed (PWM)
// const int IN1 = 11;
// const int IN2 = 12;
// const int speed =160;
// void setup() {
//   Serial.begin(9600);
//   Serial.println("Attaching servo...");


//   pinMode(ENA, OUTPUT);
//   pinMode(IN1, OUTPUT);
//   pinMode(IN2, OUTPUT);
  
//   digitalWrite(IN1, HIGH);
//   digitalWrite(IN2, LOW);
//   analogWrite(ENA, speed);
//   myServo.attach(servoPin);
//   // myServo.write(90);

//   Serial.println("Servo attached and set to 90 degrees.");
// }

// void loop() {
  
// }
