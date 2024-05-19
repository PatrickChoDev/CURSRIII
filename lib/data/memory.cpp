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
  SPI.begin(SD_CARD_SCK_PIN, SD_CARD_MISO_PIN, SD_CARD_MOSI_PIN, SD_CARD_SS_PIN);
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
  // Get current timestamp
  time_t currentTime = time(nullptr);

  // Convert timestamp to string
  char timestamp[20];
  strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&currentTime));

  // Write timestamp to flight log file
  flightLogFile.print(timestamp);
  flightLogFile.print(",");
  flightLogFile.println(data);
}

void CURSRFilesystem::logData(SensorData sensorData, SensorData kalmanData)
{
  // Get current timestamp
  time_t currentTime = time(nullptr);
  char timestamp[20];
  strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&currentTime));
  flightLogFile.print(timestamp);
  flightLogFile.print(",raw_sensor_data,");
  flightLogFile.println(String(sensorData.temperature) + "," + String(sensorData.pressure) + "," + String(sensorData.accelerationX) + "," + String(sensorData.accelerationY) + "," + String(sensorData.accelerationZ) + "," + String(sensorData.gyroscopeX) + "," + String(sensorData.gyroscopeY) + "," + String(sensorData.gyroscopeZ));
  strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&currentTime));
  flightLogFile.print(timestamp);
  flightLogFile.print(",kalman_filtered_data,");
  flightLogFile.println(String(kalmanData.temperature) + "," + String(kalmanData.pressure) + "," + String(kalmanData.accelerationX) + "," + String(kalmanData.accelerationY) + "," + String(kalmanData.accelerationZ) + "," + String(kalmanData.gyroscopeX) + "," + String(kalmanData.gyroscopeY) + "," + String(kalmanData.gyroscopeZ));
}
