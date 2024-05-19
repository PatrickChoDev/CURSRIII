#ifndef ARDUINO_H
#define ARDUINO_H
#include <Arduino.h>
#endif

#ifndef KALMAN_H
#define KALMAN_H
#include <SimpleKalmanFilter.h>
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
  float altitude;
  float latitude;
  float longitude;
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
  SimpleKalmanFilter pressureKalmanFilter = SimpleKalmanFilter(1, 1, 0.01);
  SimpleKalmanFilter temperature = SimpleKalmanFilter(1, 1, 0.01);
  SimpleKalmanFilter pressure = SimpleKalmanFilter(1, 1, 0.01);
  SimpleKalmanFilter accelerationX = SimpleKalmanFilter(1, 1, 0.01);
  SimpleKalmanFilter accelerationY = SimpleKalmanFilter(1, 1, 0.01);
  SimpleKalmanFilter accelerationZ = SimpleKalmanFilter(1, 1, 0.01);
  SimpleKalmanFilter gyroscopeX = SimpleKalmanFilter(1, 1, 0.01);
  SimpleKalmanFilter gyroscopeY = SimpleKalmanFilter(1, 1, 0.01);
  SimpleKalmanFilter gyroscopeZ = SimpleKalmanFilter(1, 1, 0.01);
  SimpleKalmanFilter altitude = SimpleKalmanFilter(1, 1, 0.01);
  SensorData kalmanData;

public:
  SensorData getKalmanData();
  void setTemperature(float temp);
  void setPressure(float pres);
  void setAltitude(float alt);
  void setAcceleration(float accX, float accY, float accZ);
  void setGyroscope(float gyroX, float gyroY, float gyroZ);
};

class CURSRData
{
private:
  SensorData sensorData;
  KalmanFilterMetrics kalmanFilterMetrics;
  void log(String message);

public:
  SensorData getRawSensorData();
  SensorData getKalmanFilteredData();
  void setSensorData(SensorData sensorData);
  char *getEncodedSensorData();
  void decodeSensorData(char *encodedData);
  void setTemperature(float temperature);
  void setPressure(float pressure);
  void setAltitude(float altitude);
  void setLatitude(float latitude);
  void setLongitude(float longitude);
  void setAccelerationX(float accelerationX);
  void setAccelerationY(float accelerationY);
  void setAccelerationZ(float accelerationZ);
  void setGyroscopeX(float gyroscopeX);
  void setGyroscopeY(float gyroscopeY);
  void setGyroscopeZ(float gyroscopeZ);
};

#endif