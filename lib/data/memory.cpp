#include <memory.hpp>

void CURSRFilesystem::log(char *message)
{
#ifdef MEMORY_LOG_ENABLED
  Serial.println(message);
#endif
}

void CURSRFilesystem::setup()
{
  log("Setting up filesystem...");
  SPI.begin(18, 16, 17, SD_CARD_SS_PIN);
  SPI.setDataMode(SPI_MODE0);
  if (SD.begin(SD_CARD_SS_PIN, SPI, 40000000, "/sd", 5U, true))
  {
    this->memoryAvailable = true;
    log("Filesystem setup complete.");
  }
  else
  {
    this->memoryAvailable = false;
    log("Filesystem setup failed.");
  }
  loadFlightStage();
}

FlightStage CURSRFilesystem::getFlightStage()
{
  return flightStage;
}

void CURSRFilesystem::loadFlightStage()
{
  if (!memoryAvailable)
  {
    log("Memory not available.");
    return;
  }
  log("Loading flight stage...");
  flightStage = SD.exists("/flight_stage") ? (FlightStage)SD.open("/flight_stage").read() : PRELAUNCH;
  log("Flight stage loaded.");
}

void CURSRFilesystem::saveFlightStage()
{
  log("Saving flight stage...");
  File flightStageFile = SD.open("/flight_stage", FILE_WRITE);
  flightStageFile.write(flightStage);
  flightStageFile.close();
  log("Flight stage saved.");
}

void CURSRFilesystem::setFlightStage(FlightStage flightStage)
{
  this->flightStage = flightStage;
}

void CURSRFilesystem::writeLog(char *message)
{
  log(message);

  systemLogFile.print(time(nullptr)); // TODO Receive timestamp from GPS
  systemLogFile.print(": ");
  systemLogFile.println(message);
}

void CURSRFilesystem::logData(char *data)
{
  log(data);
  // Get current timestamp
  time_t currentTime = time(nullptr);

  // Convert timestamp to string
  char timestamp[20];
  strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&currentTime));

  // Write timestamp to flight log file
  flightLogFile.print(timestamp);
  flightLogFile.print(" ");
  flightLogFile.println(data);
}
