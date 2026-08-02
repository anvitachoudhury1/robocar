#include <Arduino.h>
#include "config.h"
#include "commands.h"
#include "network.h"
#include "led_control.h"
#include "heart_control.h"
#include "door_control.h"
#include "motor.h"
#include <ir_sensor.h>
#include <servo_control.h>

#define DEFAULT_MOTOR_SPEED 200

// Extracts an integer speed from the tail of a command string, e.g.
// "motor a forward 180" -> 180. Returns defaultSpeed if no number found.
int extractSpeed(const String &command, int defaultSpeed)
{
  int lastSpace = command.lastIndexOf(' ');
  if (lastSpace == -1)
    return defaultSpeed;

  String tail = command.substring(lastSpace + 1);
  // Confirm the tail is actually numeric before trusting it
  for (unsigned int i = 0; i < tail.length(); i++)
  {
    if (!isDigit(tail.charAt(i)))
      return defaultSpeed;
  }
  if (tail.length() == 0)
    return defaultSpeed;

  return constrain(tail.toInt(), 0, 255);
}

void handleCommand(const String &command, int sender)
{

  if (command == "led on")
  {
    ledOn();
    broadcastAll("[Client " + String(sender) + "] LED turned ON");
  }
  else if (command == "led off")
  {
    ledOff();
    broadcastAll("[Client " + String(sender) + "] LED turned OFF");
  }
  else if (command == "heart on")
  {
    heartOn();
    broadcastAll("[Client " + String(sender) + "] heart turned ON");
  }
  else if (command == "heart off")
  {
    heartOff();
    broadcastAll("[Client " + String(sender) + "] heart turned OFF");
  }
  else if (command == "status")
  {
    // Status only goes to whoever asked
    sendToClient(sender, "[Server] LED is: " + String(ledIsOn() ? "ON" : "OFF"));
    sendToClient(sender, "[Server] heart is: " + String(heartIsOn() ? "ON" : "OFF"));
    sendToClient(sender, "[Server] Blinking: " + String(ledIsBlinking() ? "YES" : "NO"));
    sendToClient(sender, "[Server] Motor A: " + motorAGetDirection() + " @ speed " + String(motorAGetSpeed()));
    sendToClient(sender, "[Server] Motor B: " + motorBGetDirection() + " @ speed " + String(motorBGetSpeed()));
  }
  else if (command == "blink")
  {
    if (!ledStartBlink())
    {
      sendToClient(sender, "[Server] Already blinking, please wait.");
    }
    else
    {
      broadcastAll("[Client " + String(sender) + "] started blink (" + String(BLINK_REPEATS) + "x)");
    }
  }
  else if (command == "who")
  {
    sendToClient(sender, "[Server] Connected clients:");
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
      if (clientConnected(i))
      {
        String line = "  - Client " + String(i);
        if (i == sender)
          line += " (you)";
        sendToClient(sender, line);
      }
    }
  }
  else if (command == "exit" || command == "quit")
  {
    sendToClient(sender, "[Server] Goodbye!");
    disconnectClient(sender);
    // network module will clean up the slot and notify others
  }
  else if (command == "door open")
  {
    doorOpen();
    broadcastAll("[Client " + String(sender) + "] door opened ");
  }
  else if (command == "door close")
  {
    doorClose();
    broadcastAll("[Client " + String(sender) + "] door closed");
  }
  // ---------------- Motor A ----------------
  else if (command.startsWith("motor a forward"))
  {
    int speed = extractSpeed(command, DEFAULT_MOTOR_SPEED);
    motorAForward(speed);
    broadcastAll("[Client " + String(sender) + "] Motor A forward @ " + String(speed));
  }
  else if (command.startsWith("motor a backward"))
  {
    int speed = extractSpeed(command, DEFAULT_MOTOR_SPEED);
    motorABackward(speed);
    broadcastAll("[Client " + String(sender) + "] Motor A backward @ " + String(speed));
  }
  else if (command == "motor a stop")
  {
    motorAStop();
    broadcastAll("[Client " + String(sender) + "] Motor A stopped");
  }
  // ---------------- Motor B ----------------
  else if (command.startsWith("motor b forward"))
  {
    int speed = extractSpeed(command, DEFAULT_MOTOR_SPEED);
    motorBForward(speed);
    broadcastAll("[Client " + String(sender) + "] Motor B forward @ " + String(speed));
  }
  else if (command.startsWith("motor b backward"))
  {
    int speed = extractSpeed(command, DEFAULT_MOTOR_SPEED);
    motorBBackward(speed);
    broadcastAll("[Client " + String(sender) + "] Motor B backward @ " + String(speed));
  }
  else if (command == "motor b stop")
  {
    motorBStop();
    broadcastAll("[Client " + String(sender) + "] Motor B stopped");
  }
  // ---------------- Both motors ----------------
  else if (command == "motor stop")
  {
    motorStopAll();
    broadcastAll("[Client " + String(sender) + "] Both motors stopped");
  }

  else if (command == "sensor sttaus")
  {
    irSensorReadAll();
    // irSensorPrintValues();

    String data = irSensorGetValuesAsString();
    broadcastAll(data);
  }
  else if (command.startsWith("servo angle"))
  {
    int angle = extractSpeed(command, 90); // reuses your existing number-parsing helper
    servoSetAngle(angle);
    broadcastAll("[Client " + String(sender) + "] Servo set to " + String(angle) + " degrees");
  }
  else if (command == "servo status")
  {
    sendToClient(sender, "[Server] Servo angle: " + String(servoGetAngle()));
  }
  else
  {
    // Unknown command — only the sender sees the error
    sendToClient(sender, "[Server] Unknown command. Try: led on, led off, status, blink, who, exit, heart on, heart off, door open, door close, motor a forward <speed>, motor a backward <speed>, motor a stop, motor b forward <speed>, motor b backward <speed>, motor b stop, motor stop, sensor sttaus");
  }
}

void showCommands(WiFiClient client)
{

  client.println("=====================================");
  client.println(" Available Commands:");
  client.println("-------------------------------------");
  client.println("  led on              - Turn LED on");
  client.println("  led off             - Turn LED off");
  client.println("  heart on            - Turn heart on");
  client.println("  heart off           - Turn heart off");
  client.println("  door open           - Open the door");
  client.println("  door close          - Close the door");
  client.println("  blink               - Blink LED (" + String(BLINK_REPEATS) + "x)");
  client.println("  status              - Show device status");
  client.println("  sensor status       - Show IR sensor readings");
  client.println("  who                 - List connected clients");
  client.println("  exit / quit         - Disconnect");
  client.println("-------------------------------------");
  client.println("  motor a forward <speed>   - Motor A forward (0-255)");
  client.println("  motor a backward <speed>  - Motor A backward (0-255)");
  client.println("  motor a stop              - Stop Motor A");
  client.println("  motor b forward <speed>   - Motor B forward (0-255)");
  client.println("  motor b backward <speed>  - Motor B backward (0-255)");
  client.println("  motor b stop              - Stop Motor B");
  client.println("  motor stop                - Stop both motors");
  client.println("  servo angle <0-180>       - Set servo angle");
  client.println("  servo status              - Show current servo angle");
  client.println("=====================================");
}
