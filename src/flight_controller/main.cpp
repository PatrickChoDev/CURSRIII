#include <flightcom.h>

CURSRSensor Sensor;
CURSRData Data;
// CURSRRadio Radio;
CURSRFilesystem Filesystem;

// void radioThread(void *pvParameters);
// void flightThread(void *pvParameters);

void setup()
{
  Serial.begin(115200);
  Filesystem.setup();
  Sensor.setup();
  // xTaskCreatePinnedToCore(
  //     flightThread,
  //     "Flight Thread",
  //     10000,
  //     NULL,
  //     1,
  //     NULL,
  //     1);
  // xTaskCreatePinnedToCore(
  //     radioThread,
  //     "Radio Thread",
  //     10000,
  //     NULL,
  //     1,
  //     NULL,
  //     0);
}

void loop() {
  Sensor.readSensor();
  Sensor.getSensorValue(&Data);
  char msg[250];
  Serial.printf(
    "%f %f %f %d %d %f %f %f %f %f %f\n",
    Data.getRawSensorData().temperature,
    Data.getRawSensorData().pressure,
    Data.getRawSensorData().altitude,
    Data.getRawSensorData().latitude,
    Data.getRawSensorData().longitude,
    Data.getRawSensorData().accelerationX,
    Data.getRawSensorData().accelerationY,
    Data.getRawSensorData().accelerationZ,
    Data.getRawSensorData().gyroscopeX,
    Data.getRawSensorData().gyroscopeY,
    Data.getRawSensorData().gyroscopeZ
    );
  snprintf(
    msg,
    sizeof(msg),
    "%f %f %f %d %d %f %f %f %f %f %f\n",
    Data.getRawSensorData().temperature,
    Data.getRawSensorData().pressure,
    Data.getRawSensorData().altitude,
    Data.getRawSensorData().latitude,
    Data.getRawSensorData().longitude,
    Data.getRawSensorData().accelerationX,
    Data.getRawSensorData().accelerationY,
    Data.getRawSensorData().accelerationZ,
    Data.getRawSensorData().gyroscopeX,
    Data.getRawSensorData().gyroscopeY,
    Data.getRawSensorData().gyroscopeZ
    );
    Filesystem.appendFile("/dataLog.txt", msg);
    delay(1000);
}

// void radioThread(void *pvParameters)
// {
//   Radio.loraSetup();
//   while (true)
//   {
//     Serial.println(Data.getEncodedSensorData());
//     Radio.send(Data.getEncodedSensorData());
//   }
//   return;
// }

// void flightThread(void *pvParameters)
// {
//   Sensor.setup();
//   Sensor.readSensor();
//   while (true)
//   {
//     Sensor.readSensor();
//     Sensor.getSensorValue(&Data);
//     Filesystem.logData(Data.getRawSensorData(), Data.getKalmanFilteredData());
//   }
//   return;
// }

