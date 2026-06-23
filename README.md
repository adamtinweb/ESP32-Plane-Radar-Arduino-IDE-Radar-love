# ESP32 Plane Radar (Arduino IDE Port)

> Arduino IDE port of the original ESP32 Plane Radar project by MatixYo.
>
> Original project:
> https://github.com/MatixYo/ESP32-Plane-Radar

Firmware for an ESP32-C3 Super Mini and a 1.28" round GC9A01 display (240×240). Shows a circular ADS-B radar around your configured location, with WiFiManager for first-time setup.

---

## Changelog

### v2.1
- **Button reliability fix:** replaced interrupt-based button detection with a polling state machine. The interrupt approach was unreliable alongside ESP32 WiFi operations (RF activity can delay or drop GPIO interrupts). The new debounce state machine reads the button directly every loop iteration, registering every press consistently.

### v2.0
- External GPIO 7 / GPIO 8 button (onboard BOOT button removed)
- Radar-Setup AP name
- Updated setup screen layout

---

## What it does

### Wi-Fi setup (if needed)

- Captive portal on AP `PlaneRadar-Setup`
- Configure Wi-Fi credentials
- Configure radar location
- Configure display options

### Radar

- Live aircraft from ADS-B data sources
- Sonar-style circular radar display
- Aircraft type and altitude labels
- Aircraft heading indicators
- Speed vectors
- Airport runway overlays (optional)

After Wi-Fi is saved, the device reconnects automatically and starts the radar display.

---

## Hardware

### Tested Hardware

- ESP32-C3 Super Mini (Tenstar)
- GC9A01 1.28" Round TFT Display (240×240)

### Wiring

| Display | ESP32-C3 Super Mini |
|----------|----------|
| VCC | 3V3 |
| GND | GND |
| RST | GPIO 0 |
| CS | GPIO 1 |
| DC | GPIO 2 |
| SDA (MOSI) | GPIO 3 |
| SCL (SCLK) | GPIO 4 |

### ESP32-C3 Super Mini Pinout

![ESP32-C3 Super Mini Pinout](ESP32%20C3%20Supermini%20Pinout.jpg)
---

## Arduino IDE Configuration

Before compiling, configure the Arduino IDE:

| Setting | Value |
|----------|----------|
| Board | ESP32C3 Dev Module |
| Flash Size | 4MB |
| USB CDC On Boot | Enabled |
| CPU Frequency | 160MHz |
| Upload Speed | 460800 |
| Partition Scheme | **Huge APP (3MB No OTA / 1MB SPIFFS)** |

### Important: Use the Huge APP Partition

The Plane Radar firmware is larger than a typical ESP32 sketch because it includes:

- LovyanGFX
- WiFiManager
- ArduinoJson
- ADS-B processing
- Airport runway database
- Radar graphics engine

Using the default partition layout will result in:

```text
Sketch too big
text section exceeds available space in board
```

To fix this:

```text
Arduino IDE
→ Tools
→ Partition Scheme
→ Huge APP (3MB No OTA / 1MB SPIFFS)
```

---

## Wi-Fi Setup Portal

### First-Time Setup

When no Wi-Fi credentials are stored:

1. Power on the device
2. Connect to:

```text
Radar-Setup
```

3. Open:

```text
http://plane-radar.local
```

or

```text
http://192.168.4.1
```

4. Enter your Wi-Fi credentials
5. Enter your radar location
6. Save settings

The device will reboot and connect automatically.

---

## Finding Your Location (Latitude and Longitude)

The radar is centred on the latitude and longitude configured in the setup portal.

The easiest way to find your coordinates is:

https://www.latlong.net/

### How to Find Coordinates

1. Visit https://www.latlong.net/
2. Enter your town, postcode, airport, or address
3. Click **Find**
4. Copy the Latitude and Longitude values

Example:

```text
Location: Romford, UK

Latitude: 51.5761
Longitude: 0.1837
```

### Entering Coordinates

Open the configuration portal:

```text
http://plane-radar.local
```

or

```text
http://<device-ip>
```

Enter the values into:

```text
Latitude
Longitude
```

Then click Save.

### Tips

- Use at least 4 decimal places
- Do not enter ° symbols
- Negative longitude values are west of Greenwich
- Negative latitude values are south of the equator

Examples:

| Location | Latitude | Longitude |
|-----------|-----------|-----------|
| London, UK | 51.5074 | -0.1278 |
| Romford, UK | 51.5761 | 0.1837 |
| New York, USA | 40.7128 | -74.0060 |
| Sydney, Australia | -33.8688 | 151.2093 |

---

## Controls

| Action | Effect |
|----------|----------|
| Short press | Cycle radar range |
| Hold for 3 seconds | Clear Wi-Fi settings and reboot into setup mode |

### External Button (GPIO 7 / GPIO 8)

A momentary push button is required — the onboard BOOT button is not used.

| Button leg | ESP32-C3 pin |
|------------|--------------|
| Leg 1 | GPIO 7 (input) |
| Leg 2 | GPIO 8 (soft GND) |

GPIO 8 is driven LOW in firmware as a soft ground rail. No external resistors needed — the internal pull-up on GPIO 7 handles this.

GPIO 7 and GPIO 8 are adjacent pins on the ESP32-C3 Super Mini, so a standard 2-pin push button can straddle them directly with no wires needed.

```
ESP32-C3 Super Mini
┌─────────────┐
│  GPIO7 [7]──┤──[BUTTON]──┤
│  GPIO8 [8]──┤────────────┘
└─────────────┘
```

---

## Radar Display

### Features

- Circular radar display
- Aircraft type labels
- Aircraft altitude labels
- Heading indicators
- Speed vectors
- Range rings
- Cardinal directions (N, E, S, W)
- Optional runway overlays

### Aircraft Labels

This Arduino IDE version displays:

```text
A320
38000
```

instead of:

```text
EZY123
A320
38000
```

Aircraft type is shown as the primary label, with altitude beneath it.

### Reduced Screen Flicker

This port includes several optimisations to reduce visible display refresh flicker:

- Cached radar background sprite
- Aircraft-only redraws
- Single buffered screen updates
- SPI transaction wrapping during display updates

The ADS-B refresh interval remains at 5 seconds.

---

## Airport Runway Overlays

The firmware contains a database of major airports.

Features:

- Runway centreline display
- Airport ICAO labels
- Optional display toggle

Airport data is stored in:

```text
data/
├── large_airports.h
└── large_airports_data.cpp
```

---

## Project Structure

```text
PlaneRadar_Arduino/
│
├── PlaneRadar_Arduino.ino
│
├── src/
│   ├── config.h
│   │
│   ├── hardware/
│   │   ├── display.cpp
│   │   ├── display.h
│   │   └── lgfx_config.hpp
│   │
│   ├── services/
│   │   ├── adsb_client.cpp
│   │   ├── adsb_client.h
│   │   ├── radar_location.cpp
│   │   ├── radar_location.h
│   │   ├── wifi_setup.cpp
│   │   └── wifi_setup.h
│   │
│   └── ui/
│       ├── radar_display.cpp
│       ├── radar_display.h
│       ├── radar_range.h
│       ├── radar_theme.h
│       ├── runway_overlay.cpp
│       ├── runway_overlay.h
│       └── status_screens.h
│
└── data/
    ├── large_airports.h
    └── large_airports_data.cpp
```

---

## Building

### Required Arduino Libraries

- LovyanGFX
- WiFiManager
- ArduinoJson

Compile using:

```text
Board: ESP32C3 Dev Module
Partition Scheme: Huge APP (3MB No OTA / 1MB SPIFFS)
USB CDC On Boot: Enabled
```

Serial Monitor:

```text
115200 baud
```

---

## Git Notes

Ignore backup files by adding a `.gitignore` file:

```gitignore
*.bak
```

Commit and push changes:

```powershell
git add -A
git commit -m "Update radar display"
git push
```

---

## Credits

This project is an Arduino IDE adaptation of the excellent original work by MatixYo:

https://github.com/MatixYo/ESP32-Plane-Radar

The original project was developed as a PlatformIO-based firmware for the ESP32-C3 Super Mini and GC9A01 round display, featuring ADS-B aircraft tracking, WiFiManager configuration, radar rendering, airport runway overlays, and aircraft data integration.

This repository focuses on:

- Porting the project to Arduino IDE
- Adapting the build system away from PlatformIO
- Updating include paths and project structure for Arduino compatibility
- Supporting the ESP32-C3 Super Mini hardware configuration used in this repository
- Preserving the original functionality wherever possible

All credit for the original Plane Radar concept, architecture, UI design, and core implementation belongs to MatixYo and the contributors to the original project.

If you find this project useful, please consider starring the original repository:

https://github.com/MatixYo/ESP32-Plane-Radar

---

## License

Please refer to the original repository for licensing details and attribution requirements.