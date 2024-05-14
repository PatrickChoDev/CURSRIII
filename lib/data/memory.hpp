#ifndef MEMORY_LOG_ENABLED
#define MEMORY_LOG_ENABLED 1
#endif

#ifndef SD_CARD_SS_PIN
#define SD_CARD_SS_PIN 5
#endif

#ifndef ARDUINO_H
#define ARDUINO_H
#include <Arduino.h>
#endif

#ifndef SD_H
#define SD_H
#include <SD.h>
#endif

#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <data.hpp>

enum FlightStage
{
  PRELAUNCH,
  ASCENT,
  DESCENT,
  POSTFLIGHT
};

class CURSRFilesystem
{
private:
  bool memoryAvailable = false;
  void log(char *message);
  FlightStage flightStage = PRELAUNCH;
  File systemLogFile = SD.open("system_log", FILE_WRITE);
  File flightLogFile = SD.open("flight_log", FILE_WRITE);
  File sensorLogFile = SD.open("sensor_log", FILE_WRITE);

public:
  void setup();
  FlightStage getFlightStage();
  void loadFlightStage();
  void saveFlightStage();
  void setFlightStage(FlightStage flightStage);
  void writeLog(char *message);
  void logData(SensorData sensorData, SensorData kalmanData);
  void logData(char *data);
};

#endif
