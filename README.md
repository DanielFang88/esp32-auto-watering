# ESP32-S3 花园自动浇水系统  

一个基于 ESP32-S3 的超低成本全无线花园自动浇水节点

## 硬件清单
- ESP32-S3 开发板（带 USB）
- 电容式土壤湿度传感器（Gravity 3.3V版）
- 12V 电磁阀 + 12V 电源
- 1路继电器模块（支持3.3V触发）
- 杜邦线若干

## 功能（开发中）
- [x] 土壤湿度实时监测
- [x] 继电器精准控制电磁阀
- [ ] 阈值自动浇水
- [ ] ESP-NOW 多节点无线组网
- [ ] 低功耗深睡
- [ ] Web配网 + OTA

## 接线图
见 [/docs/hardware_schematic.jpg](/docs/hardware_schematic.jpg)

## 烧录方法
```bash
idf.py set-target esp32s3
idf.py menuconfig
idf.py build flash monitor