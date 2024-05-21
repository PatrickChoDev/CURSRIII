#ifndef MEMORY_LOG_ENABLED
#define MEMORY_LOG_ENABLED 1
#endif

#ifndef SD_CARD_SS_PIN
#define SD_CARD_SS_PIN 15 
#endif

#ifndef SD_CARD_MISO_PIN
#define SD_CARD_MISO_PIN 12
#endif

#ifndef SD_CARD_MOSI_PIN
#define SD_CARD_MOSI_PIN 13
#endif

#ifndef SD_CARD_SCK_PIN
#define SD_CARD_SCK_PIN 14
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
  SPIClass SDSPI;
  FlightStage flightStage = PRELAUNCH;
  void log(char *message);
  File systemLogFile;
  File flightLogFile;
  File sensorLogFile;

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
