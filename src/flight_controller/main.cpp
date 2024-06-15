#include <flightcom.h>

CURSRSensor Sensor;
CURSRData Data;
CURSRRadio Radio;
CURSRFilesystem Filesystem;

TaskHandle_t flightTask;
TaskHandle_t radioTask;

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
  delay(1000);
  xTaskCreatePinnedToCore(
      flightThread,
      "Flight Thread",
      10000,
      NULL,
      tskIDLE_PRIORITY + 1,
      &flightTask,
      0);
  delay(3000);
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
    RadioPacket packet;
    packet.latitude = Data.getRawSensorData().latitude;
    packet.longitude = Data.getRawSensorData().longitude;
    packet.altitudeGPS = Data.getRawSensorData().altitudeGPS;
    packet.altitude = Data.getKalmanFilteredData().altitude;
    packet.flightStage = Filesystem.getFlightStage();
    char *packetData = (char *)&packet;
    memccpy(packetData, &packet, sizeof(packet), sizeof(packet));
    Radio.send(packetData);
    delay(50);
  }
  return;
}

void flightThread(void *pvParameters)
{
  Filesystem.systemLog("sensor", "setup finished");
  unsigned long startTime = millis();
  float maxRMS = -10;
  float pPressure = 0;
  for (;;)
  {
    Sensor.readSensor();
    Sensor.getSensorValue(&Data);
    Filesystem.logData(Data.getRawSensorData(), "raw");
    Filesystem.logData(Data.getKalmanFilteredData(), "kalman");
    float rms = sqrt(pow(Data.getKalmanFilteredData().accelerationX, 2) + pow(Data.getKalmanFilteredData().accelerationY, 2) + pow(Data.getKalmanFilteredData().accelerationZ, 2));
    switch (Filesystem.getFlightStage())
    {
    case FLIGHTSTAGE_PRELAUNCH: // Stage 0
      if (millis() - startTime > PRELAUNCH_DELAY && Data.getKalmanFilteredData().accelerationZ < -2)
      {
        startTime = millis();
        Filesystem.systemLog("flight", "stage set to BURNOUT");
        Filesystem.setFlightStage(FLIGHTSTAGE_BURNOUT);
      }
      break;

    case FLIGHTSTAGE_BURNOUT: // Stage 1
      if (rms < maxRMS)
      {
        if (startTime > BURNOUT_DELAY)
        {
          Filesystem.systemLog("flight", "stage set to COASTING");
          Filesystem.setFlightStage(FLIGHTSTAGE_COASTING);
        }
      }
      else
      {
        maxRMS = rms;
      }
      startTime = millis();
      break;

    case FLIGHTSTAGE_COASTING: // Stage 2
                               // if (Data.getKalmanFilteredData().pressure > pPressure && Data.getKalmanFilteredData().pressure < APOGEE_PRESSURE_THRESHOLD && rms < APOGEE_ACCELERATION_THRESHOLD)
      if (Data.getKalmanFilteredData().pressure > pPressure)
      {
        if (startTime > COASTING_DELAY)
        {
          Filesystem.systemLog("flight", "stage set to APOGEE");
          Filesystem.setFlightStage(FLIGHTSTAGE_APOGEE);
        }
        startTime = millis();
        break;
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
      if (Data.getKalmanFilteredData().altitude < 400)
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
      if (rms > POSTFLIGHT_ACCELERATION_THRESHOLD)
      {
        Filesystem.systemLog("flight", "stage set to POSTFLIGHT");
        Filesystem.setFlightStage(FLIGHTSTAGE_POSTFLIGHT);
        startTime = millis();
        break;
      }
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
    delay(1);
  }
}
