#include <memory.hpp>

#if MEMORY_LOG_ENABLED
#define log(x) Serial.print(x)
#else
#define log(x)
#endif

void CURSRFilesystem::setup()
{
  log("Setting up filesystem...\n");
  sprintf(systemLogFilePath, "/system_log_%d.csv", RUN_ID);
  sprintf(flightLogFilePath, "/flight_log_%d.csv", RUN_ID);
  sprintf(flightStateFilePath, "/flightstate_%d.e", RUN_ID);
  log("File paths set.\n");
  SDSPI->begin(SD_CARD_SCK_PIN, SD_CARD_MISO_PIN, SD_CARD_MOSI_PIN, SD_CARD_SS_PIN);
  if (SD.begin(SD_CARD_SS_PIN, *SDSPI))
  {
    this->memoryAvailable = true;

#if (RESET_FLIGHT)
    SD.remove(systemLogFilePath);
    SD.remove(flightLogFilePath);
    SD.remove(flightStateFilePath);
#endif

    if (SD.exists(systemLogFilePath))
    {
      log("System log file exists.\n");
    }
    else
    {
      log("System log file does not exist.\n");
      File systemLogFile = SD.open(systemLogFilePath, FILE_WRITE);
      systemLogFile.print("Timestamp,Tag,Message\n");
      systemLogFile.close();
    }

    if (SD.exists(flightLogFilePath))
    {
      log("Flight log file exists.\n");
    }
    else
    {
      log("Flight log file does not exist.\n");
      File flightLogFile = SD.open(flightLogFilePath, FILE_WRITE);
      flightLogFile.print("Timestamp,Tag,Message,Temperature,Pressure,AccelerationX,AccelerationY,AccelerationZ,GyroscopeX,GyroscopeY,GyroscopeZ\n");
      flightLogFile.close();
    }

    if (SD.exists(flightStateFilePath))
    {
      log("Flight state file exists.\n");
    }
    else
    {
      log("Flight state file does not exist.\n");
      this->saveFlightStage();
    }

    loadFlightStage();
    log("Files removed and recreated.\n");
    log("SD Card Module setup complete.\n");
  }
  else
  {
    this->memoryAvailable = false;
    log("Filesystem setup failed.\n");
  }
  uint8_t cardType = SD.cardType();
  if (cardType == CARD_NONE)
  {
    Serial.print("No SD card attached\n");
    return;
  }
  Serial.print("SD Card Type: ");
  if (cardType == CARD_MMC)
  {
    Serial.print("MMC\n");
  }
  else if (cardType == CARD_SD)
  {
    Serial.print("SDSC\n");
  }
  else if (cardType == CARD_SDHC)
  {
    Serial.print("SDHC\n");
  }
  else
  {
    Serial.print("UNKNOWN\n");
  }
}

FlightStage CURSRFilesystem::getFlightStage()
{
  return flightStage;
}

void CURSRFilesystem::loadFlightStage()
{
  if (!memoryAvailable)
  {
    log("Memory not available.\n");
    return;
  }
  log("Loading flight stage...\n");
  flightStage = SD.exists(flightStateFilePath) ? (FlightStage)SD.open(flightStateFilePath).read() : FLIGHTSTAGE_PRELAUNCH;
  log("Flight stage loaded.\n");
}

void CURSRFilesystem::saveFlightStage()
{
  log("Saving flight stage...\n");
  File flightStageFile = SD.open(flightStateFilePath, FILE_WRITE);
  flightStageFile.write(flightStage);
  flightStageFile.close();
  log("Flight stage saved.\n");
}

void CURSRFilesystem::setFlightStage(FlightStage flightStage)
{
  this->flightStage = flightStage;
  saveFlightStage();
  this->logData(SensorData(), "FlightStage", "Flight stage changed to " + flightStage);
}

void CURSRFilesystem::appendFile(const char *path, const char *message)
{
  if (!memoryAvailable)
  {
    log("Memory not available.\n");
    return;
  }
  log("Appending to file:");
  log(path);
  log("\n");
  File file = SD.open(path, FILE_APPEND);
  if (!file)
  {
    log("Failed to open file for appending\n");
    return;
  }
  if (file.print(message))
  {
    log("Message appended\n");
  }
  else
  {
    log("Append failed\n");
  }
  file.close();
}

void CURSRFilesystem::logData(SensorData sensorData, const char *tag, const char *message)
{
  // Get current timestamp
  if (!memoryAvailable)
    return;
  time_t currentTime = time(nullptr);
  char timestamp[20];
  strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&currentTime));
  File flightLogFile = SD.open(flightLogFilePath, FILE_APPEND);
  flightLogFile.print(timestamp);
  flightLogFile.print(",");
  flightLogFile.print(tag);
  flightLogFile.print(",");
  flightLogFile.print(message);
  flightLogFile.print(",");
  flightLogFile.printf("%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f\n", sensorData.temperature, sensorData.pressure, sensorData.accelerationX, sensorData.accelerationY, sensorData.accelerationZ, sensorData.gyroscopeX, sensorData.gyroscopeY, sensorData.gyroscopeZ);
  flightLogFile.close();
}

void CURSRFilesystem::systemLog(const char *tag, const char *message)
{
  if (!memoryAvailable)
    return;
  time_t currentTime = time(nullptr);
  char timestamp[20];
  strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&currentTime));
  File flightLogFile = SD.open(systemLogFilePath, FILE_APPEND);
  flightLogFile.print(timestamp);
  flightLogFile.print(",");
  flightLogFile.print(tag);
  flightLogFile.print(",");
  flightLogFile.print(message);
  flightLogFile.close();
}
