# ESP32-S3 Smart Garden Auto-Watering System

A low-cost, fully wireless, multi-node automatic plant watering system  
Built with ESP32-S3 + Capacitive Soil Moisture Sensor + 12V Solenoid Valve

## Features
- Real-time capacitive soil moisture monitoring
- Reliable 12V solenoid valve control (NC safe scheme)
- ESP-IDF native, no Arduino framework
- Ready for ESP-NOW mesh, deep-sleep, OTA, Web provisioning

## Hardware
- ESP32-S3 development board (any with USB)
- Capacitive Soil Moisture Sensor v2 (3.3–5.5V, Gravity)
- 12V solenoid valve + 12V/2A power adapter
- 1-channel relay module (3.3V logic compatible)

## Wiring
See [docs/schematic.jpg](docs/schematic.jpg)

## Quick Start
```bash
idf.py set-target esp32s3
idf.py menuconfig
idf.py build flash monitor