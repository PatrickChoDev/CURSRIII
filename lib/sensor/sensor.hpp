#ifndef SENSOR_LOG_ENABLED
#define SENSOR_LOG_ENABLED 1
#endif

#ifndef BNO055_ENABLED
#define BNO055_ENABLED 1
#endif

#ifndef SENSOR_HPP
#define SENSOR_HPP

#ifndef ARDUINO_H
#define ARDUINO_H
#include <Arduino.h>
#endif

#ifndef WIRE_H
#define WIRE_H
#include <Wire.h>
#endif

#include <Adafruit_Sensor.h>
#include "Adafruit_BMP3XX.h"

#include <data.hpp>

extern class CURSRSensor
{
private:
  Adafruit_BMP3XX bmp;
  bool bmp390Available = false;
  float temperature = 0;
  float pressure = 0;
  float altitude = 0;
  void log(String message);

public:
  CURSRSensor()
  {
    log("Sensor created");
  };
  void setup();
  void readSensor();
  void getSensorValue(CURSRData *data);
  void setBMP390Available(bool available);
  void checkBMP390Sensor();
} Sensor;

#endif