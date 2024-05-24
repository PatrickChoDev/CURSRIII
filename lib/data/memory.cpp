#include <memory.hpp>

#if MEMORY_LOG_ENABLED
  #define log(x) Serial.print(x)
#else
  #define log(x)
#endif

void CURSRFilesystem::setup()
{ 
  log("Setting up filesystem...\n");
  SDSPI->begin(SD_CARD_SCK_PIN, SD_CARD_MISO_PIN, SD_CARD_MOSI_PIN, SD_CARD_SS_PIN);
  if (SD.begin(SD_CARD_SS_PIN, *SDSPI))
  {
    this->memoryAvailable = true;
    loadFlightStage();
    log("SD Card Module setup complete.\n");
  }
  else
  {
    this->memoryAvailable = false;
    log("Filesystem setup failed.\n");
    return;
  }
  uint8_t cardType = SD.cardType();
  if(cardType == CARD_NONE){
      log("No SD card attached\n");
      return;
  }
  Serial.print("SD Card Type: ");
  if(cardType == CARD_MMC){
      log("MMC\n");
  } else if(cardType == CARD_SD){
      log("SDSC\n");
  } else if(cardType == CARD_SDHC){
      log("SDHC\n");
  } else {
      log("UNKNOWN\n");
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
  flightStage = SD.exists("/flight_stage.txt") ? (FlightStage)SD.open("/flight_stage.txt").read() : PRELAUNCH;
  log("Flight stage loaded.\n");
}

void CURSRFilesystem::saveFlightStage()
{
  log("Saving flight stage...\n");
  File flightStageFile = SD.open("/flight_stage.txt", FILE_WRITE);
  flightStageFile.write(flightStage);
  flightStageFile.close();
  log("Flight stage saved.\n");
}

void CURSRFilesystem::setFlightStage(FlightStage flightStage)
{
  this->flightStage = flightStage;
}

void CURSRFilesystem::appendFile(const char * path, const char * message){
    if(!memoryAvailable){
      log("Memory not available.\n");
      return;
    }
    log("Appending to file:"); log(path); log("\n");
    File file = SD.open(path, FILE_APPEND);
    if(!file){
        log("Failed to open file for appending\n");
        return;
    }
    if(file.print(message)){
        log("Message appended\n");
    } else {
        log("Append failed\n");
    }
    file.close();
}

// void CURSRFilesystem::writeLog(char *message)
// {
//   log(message+'\n');

//   systemLogFile.print(time(nullptr)); // TODO Receive timestamp from GPS
//   systemLogFile.print(": ");
//   systemLogFile.println(message);
// }

// void CURSRFilesystem::logData(char *data)
// {
//   // Get current timestamp
//   time_t currentTime = time(nullptr);

//   // Convert timestamp to string
//   char timestamp[20];
//   strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&currentTime));

//   // Write timestamp to flight log file
//   flightLogFile.print(timestamp);
//   flightLogFile.print(",");
//   flightLogFile.println(data);
// }

// void CURSRFilesystem::logData(SensorData sensorData, SensorData kalmanData)
// {
//   // Get current timestamp
//   time_t currentTime = time(nullptr);
//   char timestamp[20];
//   strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&currentTime));
//   flightLogFile.print(timestamp);
//   flightLogFile.print(",raw_sensor_data,");
//   flightLogFile.println(String(sensorData.temperature) + "," + String(sensorData.pressure) + "," + String(sensorData.accelerationX) + "," + String(sensorData.accelerationY) + "," + String(sensorData.accelerationZ) + "," + String(sensorData.gyroscopeX) + "," + String(sensorData.gyroscopeY) + "," + String(sensorData.gyroscopeZ));
//   strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&currentTime));
//   flightLogFile.print(timestamp);
//   flightLogFile.print(",kalman_filtered_data,");
//   flightLogFile.println(String(kalmanData.temperature) + "," + String(kalmanData.pressure) + "," + String(kalmanData.accelerationX) + "," + String(kalmanData.accelerationY) + "," + String(kalmanData.accelerationZ) + "," + String(kalmanData.gyroscopeX) + "," + String(kalmanData.gyroscopeY) + "," + String(kalmanData.gyroscopeZ));
// }
