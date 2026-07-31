#ifndef CONFIG_H
#define CONFIG_H

// ---------------- WiFi Access Point ----------------
#define WIFI_SSID "AnvitaPrasiddha"
#define WIFI_PASS "Crochets" // must be at least 8 characters

// ---------------- TCP Server ----------------
#define TCP_PORT 23
#define MAX_CLIENTS 4
#define MAX_CMD_LEN 64 // max chars per command (RAM protection)

// ---------------- Pins ----------------
// Onboard LED uses LED_BUILTIN (defined by the core).
// Reserved for future modules:
#define TRIG_PIN 9  // HC-SR04 trigger
#define ECHO_PIN 10 // HC-SR04 echo
// #define LDR_PIN A0  // LDR voltage divider junction

// ---------------- Timing ----------------
#define BLINK_INTERVAL_MS 200
#define BLINK_REPEATS 3
#define AP_STATUS_POLL_MS 1000 // how often to poll WiFi.status() for AP join/leave
#define SERVO_PIN 9

// Array 1
#define IR_A1_S1 A0
#define IR_A1_S2 A1
#define IR_A1_S3 A2
#define IR_A1_S4 A3
#define IR_A1_S5 A4

// Array 2
#define IR_A2_S1 A5
#define IR_A2_S2 2
#define IR_A2_S3 3
#define IR_A2_S4 4
#define IR_A2_S5 5

#endif // CONFIG_H
