# firefly-wallet-esp32c3

A modern implementation of the Firefly hardware old wallet using ESP32-C3 microcontroller, mainly for self-building / learning purpose

## Key Differences from Original
- **Hardware**: XIAO ESP32-C3 instead of Arduino Nano
- **Communication**: Native BLE instead of nRF24L01+
- **Storage**: NVS (Non-Volatile Storage) instead of EEPROM
- **Memory**: 400KB RAM vs 2KB (Arduino Nano)

## Hardware Requirements
- XIAO ESP32-C3 development board
- 128x64 OLED display (I2C)
- Tactile button
- 10kΩ resistor

## Attribution
Based on the original Firefly Wallet project (https://github.com/firefly/wallet).
Adapted for ESP32-C3 platform with modern BLE communication.

## Useful links
- **Original Firefly Project**: https://firefly.city/
- **ESP32-C3 Documentation**: https://docs.espressif.com/projects/esp-idf/en/latest/esp32c3/
- **XIAO ESP32-C3 Wiki**: https://wiki.seeedstudio.com/XIAO_ESP32C3_Getting_Started/
- **Wokwi Simulator**: https://wokwi.com/
- **Ethereum Development**: https://ethereum.org/developers/