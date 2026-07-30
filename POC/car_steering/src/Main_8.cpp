// #include <SPI.h>
// #include <MFRC522.h>

// #define SS_PIN 10
// #define RST_PIN UINT8_MAX

// MFRC522 mfrc522(SS_PIN, RST_PIN);

// void setup()
// {
//     Serial.begin(9600);
//     SPI.begin();
//     mfrc522.PCD_Init();

//     Serial.println("Checking MFRC522...");
//     byte version = mfrc522.PCD_ReadRegister(mfrc522.VersionReg);
//     Serial.print("Version register: 0x");
//     Serial.println(version, HEX);

//     if (version == 0x00 || version == 0xFF)
//     {
//         Serial.println("Communication FAILED — check wiring/power");
//     }
//     else
//     {
//         Serial.println("Communication OK — chip detected");
//     }
// }

// void loop() {}