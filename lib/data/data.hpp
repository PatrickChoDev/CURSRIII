#ifndef ARDUINO_H
#define ARDUINO_H
#include <Arduino.h>
#endif

#ifndef DATA_LOG_ENABLED
#define DATA_LOG_ENABLED 1
#endif

struct SensorData
{
  float temperature;
  float pressure;
  float accelerationX;
  float accelerationY;
  float accelerationZ;
  float gyroscopeX;
  float gyroscopeY;
  float gyroscopeZ;
};

class CURSRData
{
private:
  SensorData sensorData;
  void log(String message);

public:
  SensorData getSensorData();
  void setSensorData(SensorData sensorData);
  char *getEncodedSensorData();
  void decodeSensorData(char *encodedData);
  void setTemperature(float temperature);
  void setPressure(float pressure);
  void setAccelerationX(float accelerationX);
  void setAccelerationY(float accelerationY);
  void setAccelerationZ(float accelerationZ);
  void setGyroscopeX(float gyroscopeX);
  void setGyroscopeY(float gyroscopeY);
  void setGyroscopeZ(float gyroscopeZ);
};
