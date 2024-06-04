#ifndef ARDUINO_H
#define ARDUINO_H
#include <Arduino.h>
#endif

#ifndef RADIO_LOG_ENABLED
#define RADIO_LOG_ENABLED 1
#endif

#ifndef RADIO_HPP
#define RADIO_HPP
#include <lora.hpp>
#include <SPI.h>
class CURSRRadio
{
private:
  SPIClass *LoRaSPI = new SPIClass(VSPI);
  SX126x LoRa;
  bool radioAvailable = false;
  char message[256];
  uint8_t msgLen = 0;

public:
  void setupTx();
  void setupRx();
  void send(char *data);
  void receive();
  char *getData();
  void log(String message);
};

#endif
