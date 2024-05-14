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
  xTaskCreatePinnedToCore(
      radioThread,
      "Radio Thread",
      10000,
      NULL,
      1,
      NULL,
      0);
  xTaskCreatePinnedToCore(
      flightThread,
      "Flight Thread",
      10000,
      NULL,
      2,
      NULL,
      1);
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
  Filesystem.setup();
  Sensor.setup();
  Sensor.readSensor();
  Data.init(Data.getRawSensorData());
  while (true)
  {
    Sensor.readSensor();
    Sensor.getSensorValue(&Data);
    Serial.println(Data.getEncodedSensorData());
    Filesystem.logData(Data.getEncodedSensorData());
    Serial.println("Flight stage: " + String(Filesystem.getFlightStage()));
    Serial.println("Pressure: " + String(Data.getKalmanFilteredData().pressure));

    delay(1000);
  }
  return;
}