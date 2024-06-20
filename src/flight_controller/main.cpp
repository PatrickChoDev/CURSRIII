#include <flightcom.h>

CURSRSensor Sensor;
CURSRData Data;
CURSRRadio Radio;
CURSRFilesystem Filesystem;

TaskHandle_t flightTask;
TaskHandle_t radioTask;

SensorData databuffer[100] = {};
SensorData kalmanbuffer[100] = {};

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
      10000,
      NULL,
      tskIDLE_PRIORITY + 1,
      &flightTask,
      0);
  delay(500);
  xTaskCreatePinnedToCore(
      radioThread,
      "Radio Thread",
      10000,
      NULL,
      tskIDLE_PRIORITY + 1,
      &radioTask,
      1);
}

void loop()
{
}

void radioThread(void *pvParameters)
{
  Filesystem.systemLog("lora", "lora ready");
  for (;;)
  {
    Radio.send(Data.getEncodedRadioPacket(Filesystem.getFlightStage()));
    delay(Filesystem.getFlightStage() == FLIGHTSTAGE_PRELAUNCH ? 1500 : 50);
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
    float rms = sqrt(pow(Data.getKalmanFilteredData().accelerationX, 2) + pow(Data.getKalmanFilteredData().accelerationY, 2) + pow(Data.getKalmanFilteredData().accelerationZ, 2));
    Serial.printf("Flightstage: %d, RMS: %f Pressure: %f Altitude: %f\r\n", Filesystem.getFlightStage(), rms, Data.getKalmanFilteredData().pressure, Data.getKalmanFilteredData().altitude);
    switch (Filesystem.getFlightStage())
    {
    case FLIGHTSTAGE_PRELAUNCH: // Stage 0
      if (millis() - startTime > PRELAUNCH_DELAY && rms > 150)
      {
        for (int i = buffer_idx; i < 100; i++)
        {
          Filesystem.logData(databuffer[i], "raw", "prelaunch buffer");
          Filesystem.logData(kalmanbuffer[i], "kalman", "prelaunch buffer");
        }
        for (int i = 0; i < buffer_idx; i++)
        {
          Filesystem.logData(databuffer[i], "raw", "prelaunch buffer");
          Filesystem.logData(kalmanbuffer[i], "kalman", "prelaunch buffer");
        }
        Filesystem.logData(Data.getKalmanFilteredData(), "kalman", "launch");
        Filesystem.logData(Data.getRawSensorData(), "raw", "launch");
        startTime = millis();
        Filesystem.systemLog("flight", "stage set to BURNOUT");
        Filesystem.setFlightStage(FLIGHTSTAGE_BURNOUT);
      }
      break;

    case FLIGHTSTAGE_BURNOUT: // Stage 1
      if (rms < maxRMS - 8)
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
      if (Data.getKalmanFilteredData().pressure < APOGEE_PRESSURE_THRESHOLD && Data.getKalmanFilteredData().pressure > pPressure)
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
      else if (Data.getKalmanFilteredData().pressure < APOGEE_PRESSURE_THRESHOLD)
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
      if (Data.getKalmanFilteredData().altitude < 83500)
      {
        digitalWrite(MAIN_PARACHUTE_EJECTION_PIN, HIGH);
        Filesystem.systemLog("flight", "stage set to LANDING");
        Filesystem.setFlightStage(FLIGHTSTAGE_LANDING);
        startTime = millis();
        break;
      }
      break;

    case FLIGHTSTAGE_LANDING: // Stage 5
      if (startTime > MAIN_PARACHUTE_HIGH_DELAY)
        digitalWrite(MAIN_PARACHUTE_EJECTION_PIN, LOW);
      Filesystem.systemLog("flight", "stage set to POSTFLIGHT");
      Filesystem.setFlightStage(FLIGHTSTAGE_POSTFLIGHT);
      startTime = millis();
      break;
      break;

    case FLIGHTSTAGE_POSTFLIGHT: // Stage 6
      Filesystem.logData(Data.getKalmanFilteredData(), "kalman", "postflight");
      Filesystem.logData(Data.getRawSensorData(), "raw", "postflight");
      if (millis() - startTime > POSTFLIGHT_DELAY && abs(Data.getKalmanFilteredData().pressure - pPressure) > 0.1 * pPressure)
      {
        vTaskDelete(flightTask);
      }
      break;
    default:
      break;
    }
    pPressure = Data.getKalmanFilteredData().pressure;
    if (Filesystem.getFlightStage() != FLIGHTSTAGE_PRELAUNCH)
    {
      Filesystem.logData(Data.getRawSensorData(), "raw");
      Filesystem.logData(Data.getKalmanFilteredData(), "kalman");
    }
    else
    {
      databuffer[buffer_idx] = Data.getRawSensorData();
      databuffer[buffer_idx] = Data.getKalmanFilteredData();
      buffer_idx++;
      if (buffer_idx == 100)
      {
        Filesystem.logData(Data.getRawSensorData(), "raw", "prelaunch");
        Filesystem.logData(Data.getKalmanFilteredData(), "kalman", "prelaunch");
        buffer_idx = 0;
      }
    }
  }
}
