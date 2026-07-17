#include <Arduino.h>
#include "config.h"
#include "commands.h"
#include "network.h"
#include "led_control.h"
#include "heart_control.h"
#include "door_control.h"
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
  else
  {
    // Unknown command — only the sender sees the error
    sendToClient(sender, "[Server] Unknown command. Try: led on, led off, status, blink, who, exit, heart on, heart off, door open, door close ");
  }
}
