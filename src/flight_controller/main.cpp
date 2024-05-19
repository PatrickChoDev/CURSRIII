#include <flightcom.h>

CURSRSensor Sensor;
CURSRData Data;
CURSRRadio Radio;
CURSRFilesystem Filesystem;

void radioThread(void *pvParameters);
void flightThread(void *pvParameters);

void setup()
{
  Serial.begin(115200);
  Filesystem.setup();
  xTaskCreatePinnedToCore(
      flightThread,
      "Flight Thread",
      10000,
      NULL,
      1,
      NULL,
      1);
  xTaskCreatePinnedToCore(
      radioThread,
      "Radio Thread",
      10000,
      NULL,
      1,
      NULL,
      0);
}

void loop() {}

void radioThread(void *pvParameters)
{
  Radio.loraSetup();
  while (true)
  {
    Serial.println(Data.getEncodedSensorData());
    Radio.send(Data.getEncodedSensorData());
  }
  return;
}

void flightThread(void *pvParameters)
{
  Sensor.setup();
  Sensor.readSensor();
  while (true)
  {
    Sensor.readSensor();
    Sensor.getSensorValue(&Data);
    Filesystem.logData(Data.getRawSensorData(), Data.getKalmanFilteredData());
  }
  return;
}