#ifndef MEMORY_LOG_ENABLED
#define MEMORY_LOG_ENABLED 0
#endif

#ifndef SD_CARD_SS_PIN
#define SD_CARD_SS_PIN 15
#endif

#ifndef SD_CARD_MISO_PIN
#define SD_CARD_MISO_PIN 25
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

#ifndef RUN_ID
#define RUN_ID 4
#endif

#ifndef RESET_FLIGHT
#define RESET_FLIGHT false
#endif

#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <data.hpp>

enum FlightStage
{
  FLIGHTSTAGE_PRELAUNCH,
  FLIGHTSTAGE_BURNOUT,
  FLIGHTSTAGE_COASTING,
  FLIGHTSTAGE_APOGEE,
  FLIGHTSTAGE_DESCENT,
  FLIGHTSTAGE_LANDING,
  FLIGHTSTAGE_POSTFLIGHT
};

class CURSRFilesystem
{
private:
  bool memoryAvailable = false;
  SPIClass *SDSPI = new SPIClass(HSPI);
  FlightStage flightStage = FLIGHTSTAGE_PRELAUNCH;

public:
  char systemLogFilePath[50];
  char flightLogFilePath[50];
  char flightStateFilePath[50];
  void setup();
  FlightStage getFlightStage();
  void loadFlightStage();
  void saveFlightStage();
  void setFlightStage(FlightStage flightStage);
  void appendFile(const char *path, const char *message);
  void logData(SensorData data, const char *tag = "", const char *message = "");
  void systemLog(const char *tag = "", const char *message = "");
};

#endif
