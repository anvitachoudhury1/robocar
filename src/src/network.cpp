#include <Arduino.h>
#include "config.h"
#include "network.h"
#include "commands.h"

static WiFiServer server(TCP_PORT);
static WiFiClient clients[MAX_CLIENTS];
static bool slotInUse[MAX_CLIENTS];
static String inputBuffer[MAX_CLIENTS];

static uint8_t lastApStatus = WL_AP_LISTENING;
static unsigned long lastApStatusCheck = 0;

// ---------------- Internal helpers ----------------

// WiFiS3 exposes no station list or DHCP lease info: WiFi.status() only
// flips WL_AP_LISTENING <-> WL_AP_CONNECTED when the first device joins or
// the last one leaves. A station's IP is only knowable once it opens a TCP
// connection (see acceptNewClients). Polling is throttled because each
// status() call is a full AT round-trip to the ESP32-S3 modem.
static void checkApStationEvents() {
  if (millis() - lastApStatusCheck < AP_STATUS_POLL_MS) return;
  lastApStatusCheck = millis();

  uint8_t status = WiFi.status();
  if (status == lastApStatus) return;

  if (status == WL_AP_CONNECTED) {
    Serial.println("Device joined the WiFi hotspot (IP shown when it opens a TCP connection)");
  } else if (status == WL_AP_LISTENING) {
    Serial.println("Device left the WiFi hotspot");
  }
  lastApStatus = status;
}

static void acceptNewClients() {
  WiFiClient newClient = server.available();
  if (!newClient) return;

  for (int i = 0; i < MAX_CLIENTS; i++) {
    if (!slotInUse[i]) {
      clients[i] = newClient;
      slotInUse[i] = true;
      inputBuffer[i] = "";

      Serial.print("Client connected in slot ");
      Serial.print(i);
      Serial.print(" from IP ");
      Serial.println(clients[i].remoteIP());

      clients[i].println("=====================================");
      clients[i].println(" Connected to Arduino UNO R4 WiFi");
      clients[i].println(" Commands: led on | led off | status");
      clients[i].println("           blink | who | exit");
      clients[i].println("=====================================");

      broadcastExcept("[Server] Client " + String(i) + " joined.", i);
      return;
    }
  }

  // Server full — politely reject
  Serial.println("Rejected client: server full");
  newClient.println("[Server] Sorry, server is full (max " + String(MAX_CLIENTS) + " clients). Try later.");
  newClient.flush();
  newClient.stop();
}

static void detectDisconnections() {
  for (int i = 0; i < MAX_CLIENTS; i++) {
    if (slotInUse[i] && !clients[i].connected()) {
      Serial.print("Client ");
      Serial.print(i);
      Serial.println(" disconnected");

      clients[i].stop();
      slotInUse[i] = false;
      inputBuffer[i] = "";

      broadcastAll("[Server] Client " + String(i) + " left.");
    }
  }
}

static void readClientInput() {
  for (int i = 0; i < MAX_CLIENTS; i++) {
    if (!slotInUse[i] || !clients[i].connected()) continue;

    while (clients[i].available()) {
      char c = clients[i].read();

      if (c == '\n') {
        String command = inputBuffer[i];
        inputBuffer[i] = "";
        command.trim();
        command.toLowerCase();

        // Empty line (user just pressed Enter) — ignore quietly
        if (command.length() == 0) continue;

        Serial.print("Client ");
        Serial.print(i);
        Serial.print(" sent: ");
        Serial.println(command);

        handleCommand(command, i);

      } else if (c != '\r') {
        // Cap buffer length to protect RAM against flooding
        if (inputBuffer[i].length() < MAX_CMD_LEN) {
          inputBuffer[i] += c;
        }
      }
    }
  }
}

// ---------------- Public API ----------------

void networkInit() {
  for (int i = 0; i < MAX_CLIENTS; i++) {
    slotInUse[i] = false;
    inputBuffer[i] = "";
  }

  Serial.println("Creating Access Point...");
  int status = WiFi.beginAP(WIFI_SSID, WIFI_PASS);

  if (status != WL_AP_LISTENING) {
    Serial.println("FATAL: Failed to create access point");
    while (true) {
      ;  // halt — nothing works without the AP
    }
  }

  delay(1000);  // let the AP settle

  Serial.println("Access Point started!");
  Serial.print("SSID: ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("TCP command server listening on port ");
  Serial.println(TCP_PORT);

  server.begin();

  lastApStatus = WL_AP_LISTENING;
  lastApStatusCheck = millis();
}

void networkUpdate() {
  checkApStationEvents();
  acceptNewClients();
  detectDisconnections();
  readClientInput();
}

void broadcastAll(const String &message) {
  Serial.println(message);
  for (int i = 0; i < MAX_CLIENTS; i++) {
    if (slotInUse[i] && clients[i].connected()) {
      clients[i].println(message);
    }
  }
}

void broadcastExcept(const String &message, int exceptIndex) {
  Serial.println(message);
  for (int i = 0; i < MAX_CLIENTS; i++) {
    if (i != exceptIndex && slotInUse[i] && clients[i].connected()) {
      clients[i].println(message);
    }
  }
}

void sendToClient(int index, const String &message) {
  if (index < 0 || index >= MAX_CLIENTS) return;
  if (slotInUse[index] && clients[index].connected()) {
    clients[index].println(message);
  }
}

bool clientConnected(int index) {
  if (index < 0 || index >= MAX_CLIENTS) return false;
  return slotInUse[index] && clients[index].connected();
}

void disconnectClient(int index) {
  if (index < 0 || index >= MAX_CLIENTS) return;
  if (slotInUse[index]) {
    clients[index].flush();
    clients[index].stop();
    // detectDisconnections() will finish cleanup + notify others next cycle
  }
}
