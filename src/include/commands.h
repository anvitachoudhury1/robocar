#ifndef COMMANDS_H
#define COMMANDS_H

#include <Arduino.h>

// Parse and execute a single command line received from a client.
// `command` is already trimmed and lowercased by the network layer.
// `sender` is the client slot index that sent it.
void handleCommand(const String &command, int sender);

#endif // COMMANDS_H
