#ifndef ARDUINO_H
#define ARDUINO_H
#include <Arduino.h>
#endif

#ifndef SOFTWARESERIAL_H
#define SOFTWARESERIAL_H
#include <SoftwareSerial.h>
#endif

#ifndef CAMERA_HPP
#define CAMERA_HPP
class CURSRCamera
{
private:
  int8_t TX_PIN;
  int8_t RX_PIN;
  uint32_t BAUD_RATE;
  bool cameraAvailable = false;
  uint8_t crc8(uint8_t *data, uint8_t len);

public:
  EspSoftwareSerial::UART cameraSerial;
  void begin(int8_t txPin, int8_t rxPin, uint32_t baudRate);
  void sendCommand(uint8_t *command, uint8_t len);
  void write(uint8_t *data, uint8_t len = 1);
  void flush();
  uint8_t *readResponse();
  size_t read(uint8_t *buffer, size_t len);
  bool isAvailable();
};

uint8_t calculate_cr8x_fast(uint8_t *data, size_t len);
#endif
