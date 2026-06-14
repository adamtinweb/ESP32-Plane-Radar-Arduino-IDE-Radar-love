# PlaneRadar_Arduino

Arduino IDE port for ESP32-C3 Super Mini / Tenstar + 1.28" GC9A01 round TFT.

## Arduino IDE libraries
Install these from Library Manager:
- LovyanGFX
- WiFiManager by tzapu
- ArduinoJson by Benoit Blanchon

## Board
Tools > Board: ESP32C3 Dev Module

Suggested settings:
- USB CDC On Boot: Enabled
- CPU Frequency: 160 MHz
- Flash Size: 4MB
- Partition Scheme: Default 4MB
- Upload Speed: 115200 or 460800
- Serial Monitor: 115200

## Wiring
GC9A01 display to ESP32-C3 Super Mini:
- VCC -> 3V3
- GND -> GND
- RST -> GPIO0
- CS -> GPIO1
- DC -> GPIO10
- SDA/MOSI -> GPIO3
- SCL/SCLK -> GPIO4
- BL -> 3V3

## Notes
This Arduino IDE port disables the embedded smooth VLW font used by the PlatformIO build.
The display uses built-in bitmap fonts instead.
