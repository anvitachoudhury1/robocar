# Smart Device Server — Arduino UNO R4 WiFi

A multi-client raw TCP command server running on the Arduino UNO R4 WiFi.
The board creates its own WiFi hotspot (Access Point); up to 4 clients can
connect simultaneously from any terminal and send commands. Results are
broadcast to all connected clients in real time.

## Features

- WiFi Access Point mode — no router needed, devices connect directly
- Raw TCP server on port 23 (works with `nc`, `telnet`, Termux, Termius)
- Up to 4 simultaneous clients with join/leave notifications
- Broadcast: any client's command result is shown to everyone
- Non-blocking blink animation (other clients stay responsive)
- Graceful handling of abrupt disconnects, server-full, unknown commands,
  empty input, and input flooding (64-char command cap)

## Commands

| Command   | Effect                                        | Visibility     |
|-----------|-----------------------------------------------|----------------|
| `led on`  | Turn onboard LED on                           | Broadcast      |
| `led off` | Turn onboard LED off                          | Broadcast      |
| `blink`   | Blink LED 3 times (non-blocking)              | Broadcast      |
| `status`  | Show LED + blink state                        | Sender only    |
| `who`     | List connected clients                        | Sender only    |
| `exit`    | Disconnect (others are notified)              | —              |

## Project structure

```
smart-device-server/
├── platformio.ini          PlatformIO config (board, framework, libs)
├── include/
│   ├── config.h            All constants: WiFi, ports, pins, timings
│   ├── network.h           Network module API
│   ├── commands.h          Command handler API
│   └── led_control.h       LED module API
├── src/
│   ├── main.cpp            setup() + loop() orchestration only
│   ├── network.cpp         AP setup, client slots, connect/disconnect, input
│   ├── commands.cpp        Command parsing and execution
│   └── led_control.cpp     LED state + non-blocking blink state machine
└── test/                   (placeholder for unit tests)
```

## Build & upload

### Option A: PlatformIO (recommended)

1. Install VS Code + the PlatformIO IDE extension.
2. Open this folder in VS Code (PlatformIO detects `platformio.ini`).
3. Click the PlatformIO "Upload" button (or run `pio run -t upload`).
4. Open the serial monitor: `pio device monitor` (9600 baud).

### Option B: Arduino IDE

Arduino IDE expects a single sketch folder with an `.ino` file:

1. Create a folder named `SmartDeviceServer`.
2. Copy `src/main.cpp` into it renamed as `SmartDeviceServer.ino`
   (remove the `#include <Arduino.h>` line — the IDE adds it implicitly).
3. Copy all other `src/*.cpp` and `include/*.h` files into the same folder.
4. In each `.cpp`/`.h` file, keep the includes as-is (they resolve within
   the sketch folder).
5. Select board **Arduino UNO R4 WiFi**, then Upload.

## Usage

1. After upload, open Serial Monitor at **9600 baud** — note the IP address
   (typically `192.168.4.1`).
2. Connect your computer/phone WiFi to **ArduinoHotspot**
   (password: `12345678` — change both in `include/config.h`).
3. From a terminal:

   ```
   nc 192.168.4.1 23
   ```

4. Type commands. Open a second terminal from another device to see
   broadcasts and join/leave notifications in action.

### From a phone

- **Android:** Termux (`pkg install netcat-openbsd`, then `nc 192.168.4.1 23`)
- **iOS:** Termius or any telnet/raw-TCP client app (host `192.168.4.1`, port `23`)

## Extending

- New commands: add branches in `src/commands.cpp` — the network layer
  hands you a trimmed, lowercased command string and the sender slot.
- New hardware modules (sensors, servo, display): create a matching
  `include/<module>.h` + `src/<module>.cpp`, put pins in `config.h`,
  expose an `update()` if it needs periodic work, and call it from `loop()`.
- Keep everything non-blocking (use `millis()`, never long `delay()`s in
  `loop()`-reachable code) so all clients stay responsive.

## Notes

- WPA2 AP password must be ≥ 8 characters or `WiFi.beginAP()` fails.
- The R4 WiFi AP mode reliably supports ~4 stations; `MAX_CLIENTS` is
  matched to that and to the board's 32 KB RAM budget.
- The hotspot is a local-only network — devices connected to it will not
  have internet access.
