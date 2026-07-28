// #include <Arduino.h>
// #include <SPI.h>
// #include <MFRC522.h>
// #include <Servo.h>

// #define SS_PIN 10
// #define RST_PIN 9
// #define SERVO_PIN 6

// MFRC522 rfid(SS_PIN, RST_PIN);
// Servo lockServo;

// // Replace these with YOUR RFID card UID
// byte allowedUID[] = {0xCB, 0x42, 0x19, 0x65};

// bool isAuthorized()
// {
//   if (rfid.uid.size != 4)
//     return false;

//   for (byte i = 0; i < 4; i++)
//   {
//     if (rfid.uid.uidByte[i] != allowedUID[i])
//       return false;
//   }

//   return true;
// }

// void printUID()
// {
//   Serial.print("UID: ");

//   for (byte i = 0; i < rfid.uid.size; i++)
//   {
//     if (rfid.uid.uidByte[i] < 0x10)
//       Serial.print("0");

//     Serial.print(rfid.uid.uidByte[i], HEX);
//     Serial.print(" ");
//   }

//   Serial.println();
// }

// void setup()
// {
//   Serial.begin(115200);
//   Serial.begin(115200);
//   while (!Serial)
//     ;
//   ;

//   SPI.begin();
//   rfid.PCD_Init();

//   lockServo.attach(SERVO_PIN);
//   lockServo.write(0); // Locked position

//   Serial.println("RFID Door Lock Ready");
//   Serial.println("Scan your card...");
// }

// void loop()
// {
//   if (!rfid.PICC_IsNewCardPresent())
//     return;

//   if (!rfid.PICC_ReadCardSerial())
//     return;

//   printUID();

//   if (isAuthorized())
//   {
//     Serial.println("Access Granted");

//     lockServo.write(90); // Unlock
//     delay(3000);         // Keep unlocked for 3 seconds
//     lockServo.write(0);  // Lock again

//     Serial.println("Door Locked");
//   }
//   else
//   {
//     Serial.println("Access Denied");
//   }

//   rfid.PICC_HaltA();
//   rfid.PCD_StopCrypto1();

//   delay(500);
// }