#include <flightcom.h>

CURSRSensor Sensor;
CURSRData Data;
CURSRRadio Radio;
CURSRFilesystem Filesystem;

void radioThread(void *pvParameters);
void flightThread(void *pvParameters);

void setup()
{
  Wire.begin(33, 32);
  Wire.setClock(400000);
  Serial.begin(115200);
  // Sensor.setup();
  // Filesystem.setup();
  // Radio.loraSetup();
  delay(1000);
  xTaskCreatePinnedToCore(
      flightThread,
      "Flight Thread",
      10000,
      NULL,
      1,
      NULL,
      1);
  delay(3000);
  xTaskCreatePinnedToCore(
      radioThread,
      "Radio Thread",
      10000,
      NULL,
      tskIDLE_PRIORITY,
      NULL,
      0);
}

void loop()
{
  yield();
}

void radioThread(void *pvParameters)
{
  Radio.loraSetup();
  for (;;)
  {
    Radio.send(Data.getEncodedSensorData());
    delay(1);
  }
  return;
}

void flightThread(void *pvParameters)
{
  Filesystem.setup();
  Sensor.setup();
  unsigned long startTime = millis();
  float maxRMS = -infinityf();
  float pPressure = 0;
  for (;;)
  {
    Sensor.readSensor();
    Sensor.getSensorValue(&Data);
    // Filesystem.appendFile("/raw_log.txt", Data.getEncodedSensorData());
    // Filesystem.appendFile("/kalman_log.txt", Data.getEncodedKalmanData());
    Serial.printf("Flight stage: %d\n", Filesystem.getFlightStage());
    Serial.printf("Altitude: %f\n", Data.getKalmanFilteredData().altitude);
    Serial.printf("Acceleration: %f %f %f\n", Data.getKalmanFilteredData().accelerationX, Data.getKalmanFilteredData().accelerationY, Data.getKalmanFilteredData().accelerationZ);
    float rms = sqrt(pow(Data.getKalmanFilteredData().accelerationX, 2) + pow(Data.getKalmanFilteredData().accelerationY, 2) + pow(Data.getKalmanFilteredData().accelerationZ, 2));
    Filesystem.logData(Data.getKalmanFilteredData());
    switch (Filesystem.getFlightStage())
    {
    case FLIGHTSTAGE_PRELAUNCH: // Stage 0
      if (millis() - startTime > 2000 && Data.getKalmanFilteredData().accelerationX > 0)
      {
        Filesystem.setFlightStage(FLIGHTSTAGE_BURNOUT);
      }
      break;

    case FLIGHTSTAGE_BURNOUT: // Stage 1
      if (rms < maxRMS)
      {
        maxRMS = rms;
        if (startTime > BURNOUT_DELAY)
        {
          Filesystem.setFlightStage(FLIGHTSTAGE_COASTING);
        }
        else
          startTime = millis();
        break;
      }
      break;

    case FLIGHTSTAGE_COASTING: // Stage 2
      if (rms < APOGEE_ACCELERATION_THRESHOLD || Data.getKalmanFilteredData().pressure < pPressure)
      {
        if (startTime > COASTING_DELAY)
        {
          Filesystem.setFlightStage(FLIGHTSTAGE_APOGEE);
        }
        else
          startTime = millis();
        break;
      }
      break;

    case FLIGHTSTAGE_APOGEE: // Stage 3
      // TODO Eject parachute
      Filesystem.setFlightStage(FLIGHTSTAGE_DESCENT);
      break;

    case FLIGHTSTAGE_DESCENT: // Stage 4
      if (Data.getKalmanFilteredData().altitude < 0 || rms < POSTFLIGHT_ACCELERATION_THRESHOLD)
      {
        Filesystem.setFlightStage(FLIGHTSTAGE_POSTFLIGHT);
        break;
      }
      break;

    default:
      break;
    }
  }
  pPressure = Data.getKalmanFilteredData().pressure;
  delay(1);
}
