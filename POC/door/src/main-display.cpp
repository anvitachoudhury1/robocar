// #include <Wire.h>
// #include <Adafruit_GFX.h>
// #include <Adafruit_SSD1306.h>

// #define SCREEN_WIDTH 128
// #define SCREEN_HEIGHT 64
// #define OLED_RESET -1
// #define SCREEN_ADDRESS 0x3D
// // try 0x3D if this doesn't work

// Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// void setup()
// {
//     Serial.begin(9600); // use 115200 if this is the ESP8266

//     if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
//     {
//         Serial.println("OLED not found!");
//         while (true)
//             ;
//     }

//     display.clearDisplay();
//     display.setTextSize(2);
//     display.setTextColor(SSD1306_WHITE);
//     display.setCursor(0, 20);
//     display.println("anvita!");
//     display.display(); // required — pushes buffer to screen
// }

// void loop()
// {
// }