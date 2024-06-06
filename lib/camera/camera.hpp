#ifndef ARDUINO_H
#define ARDUINO_H
#include <Arduino.h>
#endif

#ifndef CAMERA_HPP
#define CAMERA_HPP

void getDeviceInfo();

void sendStartCommand();
void sendStopCommand();
void sendSwitchCommand();
void sendPowerButton();
void sendOSDCommand();

void pressKeyCommand(uint8_t key);

uint8_t crc8_calc(uint8_t crc, unsigned char a, uint8_t poly);

uint8_t calcCrc(uint8_t *buf, uint8_t numBytes);

#endif
