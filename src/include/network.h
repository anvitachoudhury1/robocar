#ifndef NETWORK_H
#define NETWORK_H

#include <WiFiS3.h>

// Initialize the WiFi Access Point and start the TCP server.
// Halts (infinite loop) with a Serial error message if the AP fails to start.
void networkInit();

// Call every loop() cycle:
// - accepts new clients (or rejects when full)
// - detects and cleans up disconnected clients
// - reads incoming characters into per-client buffers, dispatching
//   complete commands to the command handler
void networkUpdate();

// Send a message to ALL connected clients (also logged to Serial).
void broadcastAll(const String &message);

// Send a message to all connected clients EXCEPT one slot index.
void broadcastExcept(const String &message, int exceptIndex);

// Send a message to a single client slot (safe: checks connection first).
void sendToClient(int index, const String &message);

// True if the given slot currently holds a connected client.
bool clientConnected(int index);

// Gracefully disconnect a client slot (used by the "exit" command).
void disconnectClient(int index);

#endif // NETWORK_H
