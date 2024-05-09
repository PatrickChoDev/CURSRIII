#include <flightcom.h>

#define SENSOR_LOG_ENABLED 0

CURSRSensor Sensor;
CURSRData Data;
CURSRRadio Radio;

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
      1,
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
}

void flightThread(void *pvParameters)
{
  Sensor.setup();
  Sensor.readSensor();
  Sensor.getSensorValue(&Data);
  Data.init(Data.getRawSensorData());
  while (true)
  {
    Sensor.readSensor();
    Sensor.getSensorValue(&Data);
  }
}