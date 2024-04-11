#include <flightcom.h>

#define SENSOR_LOG_ENABLED 0

CURSRSensor Sensor;
CURSRData Data;

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
  while (true)
  {
    Serial.println("Value from Radio Thread: " + String(Sensor.getSensorValue()));
    delay(1000);
  }
}

void flightThread(void *pvParameters)
{
  Sensor.setup();
  while (true)
  {
    Sensor.readSensor();

    Serial.println("Value from Flight Thread: " + String(Sensor.getSensorValue()));
    delay(200);
  }
}