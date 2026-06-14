# Plane Radar — Arduino IDE Port

ESP32-Plane-Radar-Arduino-IDE-Radar-love

Arduino IDE firmware for an **ESP32-C3 Super Mini** and a **1.28″ round GC9A01 TFT display** (240×240).  
Shows a circular ADS-B aircraft radar around your configured location, using WiFiManager for first-time setup.

This version is adapted for the Arduino IDE and uses `.ino`-based sketch files rather than the original PlatformIO `src/main.cpp` layout.

---

## What it does

- **Wi‑Fi setup** — captive portal on AP `PlaneRadar-Setup` when no saved Wi‑Fi is found
- **Radar display** — live aircraft from `adsb.fi` on a circular sonar-style grid
- **Automatic reconnect** — saved Wi‑Fi credentials are reused after reboot
- **Range control** — short BOOT press cycles radar range presets
- **Reset control** — long BOOT press clears saved Wi‑Fi/location settings

After Wi‑Fi is saved, the device reconnects automatically and the radar runs from the main Arduino loop with periodic ADS-B updates.

---

## Hardware

### Board

Tested/targeted for:

```text
ESP32-C3 Super Mini / Tenstar ESP32-C3 Super Mini
```

Arduino IDE board selection:

```text
Tools → Board → ESP32 Arduino → ESP32C3 Dev Module
```

Recommended settings:

```text
USB CDC On Boot: Enabled
CPU Frequency: 160 MHz
Flash Size: 4MB
Partition Scheme: Huge APP / No OTA / Minimal SPIFFS
Upload Speed: 460800 or 115200
Serial Monitor: 115200 baud
```

The firmware is quite large, so the default partition may be too small. Use a larger app partition such as **Huge APP** or **No OTA**.

---

## Wiring — GC9A01 ↔ ESP32-C3 Super Mini

| Display | ESP32-C3 Super Mini |
|---|---|
| VCC | 3V3 |
| GND | GND |
| RST | GPIO 0 |
| CS | GPIO 1 |
| DC | GPIO 10 |
| SDA / MOSI | GPIO 3 |
| SCL / SCLK | GPIO 4 |
| BL | 3V3 |
| BOOT button | GPIO 9 |

Display pins are configured in:

```text
src/config.h
src/hardware/lgfx_config.hpp
```

---

## Controls — BOOT button, GPIO 9, active LOW

| Action | Effect |
|---|---|
| Short tap | Cycle range preset: 5 → 10 → 15 → 25 km |
| Hold 3 seconds | Clear Wi‑Fi, location, and saved settings, then reboot into setup portal |
| Hold BOOT at power-on | Force credential reset / setup mode |

Settings are saved to flash/NVS so they persist after reboot.

---

## Wi‑Fi setup portal

### First-time setup

When no saved Wi‑Fi exists, the ESP32 creates this access point:

```text
PlaneRadar-Setup
```

Connect to it with your phone or laptop, then open:

```text
http://192.168.4.1
```

or:

```text
http://plane-radar.local
```

The captive portal may also open automatically.

Enter your home Wi‑Fi SSID/password and save.

---

### Reconfigure later

If the device is already on your Wi‑Fi, open:

```text
http://plane-radar.local
```

or use the IP address shown in the serial monitor or your router.

You can change:

| Field | Purpose |
|---|---|
| Latitude | Radar centre latitude |
| Longitude | Radar centre longitude |
| Display distances in miles | Shows ring labels in miles instead of km |
| Show airport runways | Enables/disables runway overlay |

---

## Location

The radar needs a latitude and longitude for its centre point.

Default values are stored in:

```text
src/config.h
```

Look for:

```cpp
constexpr double kDefaultRadarLat = ...;
constexpr double kDefaultRadarLon = ...;
```

These defaults are used until you set a location through the Wi‑Fi setup portal.

---

## Radar display

### Grid

- Dark radar-style background
- Circular rings and crosshairs
- N / S / E / W markers around the bezel
- Range label on the east spoke
- Centre dot for your configured location

### Aircraft

- Aircraft inside the outer ring are drawn as heading markers
- Speed vector shows projected movement
- Callsign / type / altitude tags are shown where available
- Aircraft outside the active ring may appear as rim direction markers

### Range presets

| Ring label | Approx outer aircraft scale |
|---|---|
| 5 km / 3 mi | ~6.7 km |
| 10 km / 6 mi | ~13.3 km |
| 15 km / 9 mi | ~20 km |
| 25 km / 16 mi | ~33.3 km |

Range preset and miles/km choice persist across reboot.

---

## ADS-B source

Aircraft data source:

```text
https://opendata.adsb.fi/api/v3/
```

Relevant settings are in:

```text
src/config.h
src/services/adsb_client.cpp
src/ui/radar_range.h
```

Typical options:

| Setting | Purpose |
|---|---|
| `kAdsbFetchIntervalMs` | ADS-B refresh interval |
| `kAdsbShowGroundAircraft` | Show/hide ground aircraft |
| `fetchRadiusKm()` | ADS-B query radius based on active range |

---

## Arduino IDE project layout

```text
PlaneRadar_Arduino/
  PlaneRadar_Arduino.ino
  src/
    config.h
    data/
      large_airports_data.cpp
    hardware/
      display.cpp
      display.h
      lgfx_config.hpp
    services/
      adsb_client.cpp
      adsb_client.h
      radar_location.cpp
      radar_location.h
      wifi_setup.cpp
      wifi_setup.h
    ui/
      radar_display.cpp
      radar_display.h
      radar_range.cpp
      radar_range.h
      runway_overlay.cpp
      runway_overlay.h
      status_screens.cpp
      status_screens.h
```

The main sketch file is:

```text
PlaneRadar_Arduino.ino
```

Arduino IDE compiles the `.ino` file plus the `.cpp` files inside `src/`.

---

## Build in Arduino IDE

1. Open:

```text
PlaneRadar_Arduino.ino
```

2. Select board:

```text
ESP32C3 Dev Module
```

3. Recommended settings:

```text
USB CDC On Boot: Enabled
Flash Size: 4MB
Partition Scheme: Huge APP / No OTA / Minimal SPIFFS
Serial Monitor: 115200 baud
```

4. Install libraries:

```text
LovyanGFX
WiFiManager
ArduinoJson
```

5. Click Upload.

If upload fails, hold **BOOT**, click Upload, then release **BOOT** when the IDE starts connecting.

---

## Dependencies

Install these from Arduino Library Manager:

- LovyanGFX
- WiFiManager
- ArduinoJson

ESP32 board package required:

```text
ESP32 by Espressif Systems
```

---

## Serial monitor

Use:

```text
115200 baud
```

Useful output includes:

- Wi‑Fi connection status
- Device IP address
- Portal status
- ADS-B fetch status
- Radar refresh messages

---

## GitHub push notes

For first upload to GitHub:

```powershell
cd "C:\Users\chiro\OneDrive\Documents\Arduino\Radar Love\radar\PlaneRadar_Arduino"
git init
git add .
git commit -m "Initial Arduino IDE port for ESP32-C3 Super Mini"
git branch -M main
git remote add origin https://github.com/adamtinweb/ESP32-Plane-Radar-Arduino-IDE-Radar-love.git
git push -u origin main
```

If GitHub already has files and you want to overwrite the remote:

```powershell
git push -u origin main --force
```

---

## Notes

This is an Arduino IDE port of the Plane Radar project.  
It keeps the radar UI and ADS-B behaviour but changes the project structure so it builds from `PlaneRadar_Arduino.ino` in Arduino IDE.
