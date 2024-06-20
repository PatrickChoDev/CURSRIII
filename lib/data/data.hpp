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
  int32_t latitude;
  int32_t longitude;
  int32_t altitudeGPS;
  float accelerationX;
  float accelerationY;
  float accelerationZ;
  float orientationX;
  float orientationY;
  float orientationZ;
  float magneticX;
  float magneticY;
  float magneticZ;
  float gyroscopeX;
  float gyroscopeY;
  float gyroscopeZ;
};

struct RadioPacket
{
  int32_t latitude;
  int32_t longitude;
  int32_t altitudeGPS;
  float altitude;
  int flightStage;
};

class KalmanFilterMetrics
{
private:
  SimpleKalmanFilter temperature = SimpleKalmanFilter(1, 1, 0.01);
  SimpleKalmanFilter pressure = SimpleKalmanFilter(1, 1, 0.03);
  SimpleKalmanFilter accelerationX = SimpleKalmanFilter(1, 1, 0.1);
  SimpleKalmanFilter accelerationY = SimpleKalmanFilter(1, 1, 0.1);
  SimpleKalmanFilter accelerationZ = SimpleKalmanFilter(1, 1, 0.1);
  SimpleKalmanFilter orientationX = SimpleKalmanFilter(1, 1, 0.01);
  SimpleKalmanFilter orientationY = SimpleKalmanFilter(1, 1, 0.01);
  SimpleKalmanFilter orientationZ = SimpleKalmanFilter(1, 1, 0.01);
  SimpleKalmanFilter magneticX = SimpleKalmanFilter(1, 1, 0.01);
  SimpleKalmanFilter magneticY = SimpleKalmanFilter(1, 1, 0.01);
  SimpleKalmanFilter magneticZ = SimpleKalmanFilter(1, 1, 0.01);
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
  void setOrientation(float orientX, float orientY, float orientZ);
  void setMagnetic(float magX, float magY, float magZ);
  void setGyroscope(float gyroX, float gyroY, float gyroZ);
  void setAltitudeGPS(int32_t alt);
  void setLatitude(int32_t lat);
  void setLongitude(int32_t lon);
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
  char *getEncodedKalmanData();
  char *getLogSensorData();
  char *getLogKalmanData();
  char *getEncodedRadioPacket(int flightStage);
  void decodeSensorData(char *encodedData);
  void setTemperature(float temperature);
  void setPressure(float pressure);
  void setAltitude(float altitude);
  void setLatitude(int32_t latitude);
  void setLongitude(int32_t longitude);
  void setAccelerationX(float accelerationX);
  void setAccelerationY(float accelerationY);
  void setAccelerationZ(float accelerationZ);
  void setOrientationX(float orientationX);
  void setOrientationY(float orientationY);
  void setOrientationZ(float orientationZ);
  void setMagneticX(float magneticX);
  void setMagneticY(float magneticY);
  void setMagneticZ(float magneticZ);
  void setGyroscopeX(float gyroscopeX);
  void setGyroscopeY(float gyroscopeY);
  void setGyroscopeZ(float gyroscopeZ);
  void setAltitudeGPS(int32_t altitudeGPS);
};

#endif