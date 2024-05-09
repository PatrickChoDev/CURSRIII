#ifndef ARDUINO_H
#define ARDUINO_H
#include <Arduino.h>
#endif

#ifndef KALMAN_H
#define KALMAN_H
#include <kalman.h>
#endif

#ifndef DATA_LOG_ENABLED
#define DATA_LOG_ENABLED 1
#endif

#ifndef DATA_HPP
#define DATA_HPP
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

class KalmanFilterMetrics
{
private:
  float temperatureTime = millis() / 1000.f;
  float pressureTime = millis() / 1000.f;
  float accelerationTime = millis() / 1000.f;
  float gyroscopeTime = millis() / 1000.f;
  KalmanFilter temperature;
  KalmanFilter pressure;
  KalmanFilter acceleration;
  KalmanFilter gyroscope;
  SensorData kalmanData;

public:
  KalmanFilterMetrics();
  void init(SensorData sensorData);
  SensorData getKalmanData();
  void setTemperature(float temp);
  void setPressure(float pres);
  void setAcceleration(float accX, float accY, float accZ);
  void setGyroscope(float gyroX, float gyroY, float gyroZ);
};

class CURSRData
{
private:
  SensorData sensorData;
  KalmanFilterMetrics kalmanFilterMetrics = KalmanFilterMetrics();
  void log(String message);

public:
  SensorData getRawSensorData();
  SensorData getKalmanFilteredData();
  void init(SensorData sensorData);
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

#endif