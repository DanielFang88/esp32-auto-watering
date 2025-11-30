<div align="center">

<img src="assets/watering_pixel.png" width="256" alt="Pixel Art Watering">

# ESP32-S3 Smart Garden Auto-Watering System

**Never let your plants die of thirst again.**

A fully open-source, pixel-art powered, ultra-low-cost automatic plant watering node  
Built from scratch with ESP32-S3 — because forgetting to water plants is unacceptable

</div>

## Core Features
- Real-time capacitive soil moisture sensing with **moving average filter** (noise killer)  
- Smart 10-second stable-dry detection (no false triggers)  
- Quantitative watering (fixed 8 s burst)  
- Safe NC relay control (failsafe when MCU crashes)  
- ESP-IDF native – zero Arduino bloat  
- Pixel-art boot animation (because aesthetics matter)

## Hardware (≈ $12 total)
| Component                              | Note                              |
|----------------------------------------|-----------------------------------|
| ESP32-S3 dev board                     | Any with USB                      |
| Capacitive Soil Moisture Sensor v2     | 3.3–5.5V Gravity version          |
| 12V solenoid valve + 12V/2A adapter    | Normally closed                   |
| 1-channel relay module                 | 3.3V logic compatible             |
| Jumper wires                           | DuPont heaven                     |

## Wiring Diagram
<img src="docs/esp32-s3_devkitc-1_pinlayout_v1.1.jpg" width="900" alt="ESP32 Garden Wiring Diagram">

## Quick Start
```bash
idf.py set-target esp32s3
idf.py menuconfig
idf.py build flash monitor