#include <flightcom.h>

CURSRSensor Sensor;
CURSRData Data;
CURSRRadio Radio;
CURSRFilesystem Filesystem;

TaskHandle_t flightTask;
TaskHandle_t radioTask;

#define BUFFER_SIZE 100
SensorData databuffer[BUFFER_SIZE] = {};
SensorData kalmanbuffer[BUFFER_SIZE] = {};

void radioThread(void *pvParameters);
void flightThread(void *pvParameters);

void setup()
{
  Wire.end();
  Wire.setPins(33, 32);
  Wire.begin(33, 32);
  Serial.begin(115200);
  Sensor.setup();
  Filesystem.setup();
  Radio.setupTx();
  xTaskCreatePinnedToCore(
      flightThread,
      "Flight Thread",
      50000,
      NULL,
      tskIDLE_PRIORITY + 1,
      &flightTask,
      0);
  delay(500);
  xTaskCreatePinnedToCore(
      radioThread,
      "Radio Thread",
      50000,
      NULL,
      tskIDLE_PRIORITY + 1,
      &radioTask,
      1);
}

void loop()
{
  vTaskDelete(NULL);
}

void radioThread(void *pvParameters)
{
  Filesystem.systemLog("lora", "lora ready");
  for (;;)
  {
    FlightStage currentRadioFlightStage = Filesystem.getFlightStage();
    Radio.send(Data.getEncodedRadioPacket(currentRadioFlightStage));
    delay(currentRadioFlightStage == FLIGHTSTAGE_PRELAUNCH ? 1500 : 50);
  }
  return;
}

void flightThread(void *pvParameters)
{
  Filesystem.systemLog("sensor", "setup finished");
  unsigned long startTime = millis();
  float maxRMS = -10;
  float pPressure = 0;
  int buffer_idx = 0;
  unsigned long coasting_lock = 0;
  for (;;)
  {
    delay(1);
    Sensor.readSensor();
    Sensor.getSensorValue(&Data);
    SensorData currentKalmanData = Data.getKalmanFilteredData();
    SensorData currentRawData = Data.getRawSensorData();
    float rms = sqrt(pow(currentKalmanData.accelerationX, 2) + pow(currentKalmanData.accelerationY, 2) + pow(currentKalmanData.accelerationZ, 2));
    FlightStage currentFlightState = Filesystem.getFlightStage();
    Serial.printf("Flightstage: %d, RMS: %f Pressure: %f Altitude: %f\r\n", currentFlightState, rms, currentKalmanData.pressure, currentKalmanData.altitude);
    switch (currentFlightState)
    {
    case FLIGHTSTAGE_PRELAUNCH: // Stage 0
      if (millis() - startTime > PRELAUNCH_DELAY && rms > 150)
      // Boost detected
      {
        // Log prelaunch data
        for (int i = buffer_idx; i < BUFFER_SIZE; i++)
        {
          Filesystem.logData(databuffer[i], "raw", "prelaunch buffer");
          Filesystem.logData(kalmanbuffer[i], "kalman", "prelaunch buffer");
        }
        for (int i = 0; i < buffer_idx; i++)
        {
          Filesystem.logData(databuffer[i], "raw", "prelaunch buffer");
          Filesystem.logData(kalmanbuffer[i], "kalman", "prelaunch buffer");
        }
        Filesystem.logData(currentKalmanData, "kalman", "launch");
        Filesystem.logData(currentRawData, "raw", "launch");
        startTime = millis();
        Filesystem.systemLog("flight", "stage set to BURNOUT");
        Filesystem.setFlightStage(FLIGHTSTAGE_BOOSTING);
      }
      break;

    case FLIGHTSTAGE_BOOSTING: // Stage 1
      if (rms < maxRMS - 8) // Motor burned out
      {
        if (startTime > BURNOUT_DELAY)
        {
          Filesystem.systemLog("flight", "stage set to COASTING");
          Filesystem.setFlightStage(FLIGHTSTAGE_COASTING);
          startTime = millis();
        }
      }
      else if (rms > maxRMS)
      {
        maxRMS = rms;
      }
      break;

    case FLIGHTSTAGE_COASTING: // Stage 2
      if (currentKalmanData.pressure < APOGEE_PRESSURE_THRESHOLD && currentKalmanData.pressure > pPressure)
      {
        if (coasting_lock == 0)
        {
          coasting_lock = millis();
        }
        else if (startTime > COASTING_DELAY && COASTING_LOCKOUT < millis() - coasting_lock)
        {
          Filesystem.systemLog("flight", "stage set to APOGEE");
          Filesystem.setFlightStage(FLIGHTSTAGE_APOGEE);
        }
      }
      else if (currentKalmanData.pressure < APOGEE_PRESSURE_THRESHOLD)
      {
        coasting_lock = millis();
      }
      break;

    case FLIGHTSTAGE_APOGEE: // Stage 3
      // TODO Eject parachute
      digitalWrite(DROUGE_PARACHUTE_EJECTION_PIN, HIGH);
      Filesystem.systemLog("flight", "stage set to DESCENT");
      Filesystem.setFlightStage(FLIGHTSTAGE_DESCENT);
      startTime = millis();
      break;

    case FLIGHTSTAGE_DESCENT: // Stage 4
      if (startTime > DROUGE_PARACHUTE_HIGH_DELAY)
        digitalWrite(DROUGE_PARACHUTE_EJECTION_PIN, LOW);
      if (currentKalmanData.altitude < 83500)
      {
        digitalWrite(MAIN_PARACHUTE_EJECTION_PIN, HIGH);
        Filesystem.systemLog("flight", "stage set to LANDING");
        Filesystem.setFlightStage(FLIGHTSTAGE_LANDING);
        startTime = millis();
      }
      break;

    case FLIGHTSTAGE_LANDING: // Stage 5
      if (startTime > MAIN_PARACHUTE_HIGH_DELAY)
        digitalWrite(MAIN_PARACHUTE_EJECTION_PIN, LOW);
      Filesystem.systemLog("flight", "stage set to POSTFLIGHT");
      Filesystem.setFlightStage(FLIGHTSTAGE_POSTFLIGHT);
      startTime = millis();
      break;

    case FLIGHTSTAGE_POSTFLIGHT: // Stage 6
      Filesystem.logData(currentKalmanData, "kalman", "postflight");
      Filesystem.logData(currentRawData, "raw", "postflight");
      if (millis() - startTime > POSTFLIGHT_DELAY && abs(currentKalmanData.pressure - pPressure) > 0.1 * pPressure)
      {
        vTaskDelete(flightTask);
      }
      break;
    default:
      break;
    }

    // Save data
    pPressure = currentKalmanData.pressure;
    if (currentFlightState == FLIGHTSTAGE_PRELAUNCH)
    {
      databuffer[buffer_idx] = currentRawData;
      databuffer[buffer_idx] = currentKalmanData;
      buffer_idx++;
      if (buffer_idx >= BUFFER_SIZE)
      {
        Filesystem.logData(currentRawData, "raw", "prelaunch");
        Filesystem.logData(currentKalmanData, "kalman", "prelaunch");
        buffer_idx = 0;
      }
    }
    else
    {
      Filesystem.logData(currentRawData, "raw");
      Filesystem.logData(currentKalmanData, "kalman");
    }
  }
}
