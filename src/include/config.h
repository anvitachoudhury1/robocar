#ifndef CONFIG_H
#define CONFIG_H

// ---------------- WiFi Access Point ----------------
#define WIFI_SSID "AnvitaPrasiddha"
#define WIFI_PASS "\
" // must be at least 8 characters

// ---------------- TCP Server ----------------
#define TCP_PORT 23
#define MAX_CLIENTS 4
#define MAX_CMD_LEN 64 // max chars per command (RAM protection)

// ---------------- Pins ----------------
// Onboard LED uses LED_BUILTIN (defined by the core).
// Reserved for future modules:

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

#define MOTOR_STBY 6  // standby (must be HIGH to enable driver)
#define MOTOR_AIN1 7  // motor A direction pin 1
#define MOTOR_AIN2 8  // motor A direction pin 2
#define MOTOR_PWMA 10 // motor A speed (PWM)
#define MOTOR_BIN1 12 // motor B direction pin 1
#define MOTOR_BIN2 13 // motor B direction pin 2
#define MOTOR_PWMB 11 // motor B speed (PWM)

#define SERVO_PIN 9

#endif // CONFIG_H
