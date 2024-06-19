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
#include <Adafruit_ADXL375.h>
#include <Adafruit_BNO055.h>
#include <SparkFun_u-blox_GNSS_v3.h>

#include <data.hpp>

extern class CURSRSensor
{
private:
  Adafruit_BMP3XX bmp;
  Adafruit_BNO055 bno;
  Adafruit_ADXL375 accel = Adafruit_ADXL375(12345);
  SFE_UBLOX_GNSS gnss;
  bool bmp390Available = false;
  bool ADXLAvailable = false;
  bool bnoAvailable = false;
  bool maxm10sAvailable = false;
  float temperature = 0;
  float pressure = 0;
  float altitude = 0;
  float accelX = 0;
  float accelY = 0;
  float accelZ = 0;
  float gyroX = 0;
  float gyroY = 0;
  float gyroZ = 0;
  uint32_t timestamp = 0;
  int32_t latitude = 0;
  int32_t longitude = 0;
  int32_t altitudeGPS = 0;
  void log(String message);
  void checkBMP390Sensor();
  void checkBNO055Sensor();
  void checkADXLSensor();
  void checkMAXM10SSensor();

public:
  void setup();
  void readSensor();
  void getSensorValue(CURSRData *data);
  void setBMP390Available(bool available);
} Sensor;

#endif
