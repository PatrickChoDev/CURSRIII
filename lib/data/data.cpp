#include <data.hpp>

/**
 * @brief Returns the Kalman filtered data.
 *
 * This function returns the Kalman filtered data.
 *
 * @return The Kalman filtered data.
 */
SensorData KalmanFilterMetrics::getKalmanData()
{
  return kalmanData;
}

/**
 * @brief Sets the temperature.
 *
 * This function sets the temperature in the Kalman filter.
 *
 * @param temp The temperature to be set.
 */
void KalmanFilterMetrics::setTemperature(float temp)
{
  kalmanData.temperature = temperature.updateEstimate(temp);
}

/**
 * @brief Sets the pressure.
 *
 * This function sets the pressure in the Kalman filter.
 *
 * @param pres The pressure to be set.
 */
void KalmanFilterMetrics::setPressure(float pres)
{
  kalmanData.pressure = pressure.updateEstimate(pres);
}

/**
 * @brief Sets the altitude.
 *
 * This function sets the altitude in the Kalman filter.
 *
 * @param alt The altitude to be set.
 */
void KalmanFilterMetrics::setAltitude(float alt)
{
  kalmanData.altitude = altitude.updateEstimate(alt);
}

/**
 * @brief Sets the acceleration.
 *
 * This function sets the acceleration in the Kalman filter.
 *
 * @param accX The acceleration in the X-axis to be set.
 * @param accY The acceleration in the Y-axis to be set.
 * @param accZ The acceleration in the Z-axis to be set.
 */
void KalmanFilterMetrics::setAcceleration(float accX, float accY, float accZ)
{
  kalmanData.accelerationX = accelerationX.updateEstimate(accX);
  kalmanData.accelerationY = accelerationY.updateEstimate(accY);
  kalmanData.accelerationZ = accelerationZ.updateEstimate(accZ);
}

/**
 * @brief Sets the gyroscope.
 *
 * This function sets the gyroscope in the Kalman filter.
 *
 * @param gyroX The gyroscope in the X-axis to be set.
 * @param gyroY The gyroscope in the Y-axis to be set.
 * @param gyroZ The gyroscope in the Z-axis to be set.
 */
void KalmanFilterMetrics::setGyroscope(float gyroX, float gyroY, float gyroZ)
{
  kalmanData.gyroscopeX = gyroscopeX.updateEstimate(gyroX);
  kalmanData.gyroscopeY = gyroscopeY.updateEstimate(gyroY);
  kalmanData.gyroscopeZ = gyroscopeZ.updateEstimate(gyroZ);
}

/**
 * @brief Logs a message to the Serial.
 *
 * This function logs a message to the Serial if the DATA_LOG_ENABLED is enabled.
 *
 * @param message The message to be logged.
 */
void CURSRData::log(String message)
{
  if (DATA_LOG_ENABLED)
  {
    Serial.println(message);
  }
}

/**
 * @brief Returns the sensor data.
 *
 * This function returns the sensor data.
 *
 * @return The sensor data.
 */
SensorData CURSRData::getRawSensorData()
{
  return sensorData;
}

/**
 * @brief Return kalman filtered data.
 *
 * This function returns the calculated sensor data.
 *
 * @return THe calculated sensor data.
 */
SensorData CURSRData::getKalmanFilteredData()
{
  return kalmanFilterMetrics.getKalmanData();
}

/**
 * @brief Sets the sensor data.
 *
 * This function sets the sensor data.
 *
 * @param sensorData The sensor data to be set.
 */
void CURSRData::setSensorData(SensorData sensorData)
{
  this->sensorData = sensorData;
  kalmanFilterMetrics.setTemperature(sensorData.temperature);
  kalmanFilterMetrics.setPressure(sensorData.pressure);
  kalmanFilterMetrics.setAcceleration(sensorData.accelerationX, sensorData.accelerationY, sensorData.accelerationZ);
  kalmanFilterMetrics.setGyroscope(sensorData.gyroscopeX, sensorData.gyroscopeY, sensorData.gyroscopeZ);
}

/**
 * @brief Encodes the sensor data.
 *
 * This function encodes the sensor data into a byte array.
 *
 * @return The encoded sensor data.
 */
char *CURSRData::getEncodedSensorData()
{
  char *encodedData = new char[sizeof(SensorData)];
  SensorData sense = this->getKalmanFilteredData();
  memcpy(encodedData, &sense, sizeof(SensorData));
  return encodedData;
}

/**
 * @brief Encodes the Kalman filtered data.
 *
 * This function encodes the Kalman filtered data into a byte array.
 *
 * @return The encoded Kalman filtered data.
 */
char *CURSRData::getEncodedKalmanData()
{
  char *encodedData = new char[sizeof(SensorData)];
  SensorData sense = this->getKalmanFilteredData();
  memcpy(encodedData, &sense, sizeof(SensorData));
  return encodedData;
}

/**
 * @brief Returns the sensor data in a log format.
 *
 * This function returns the sensor data in a log format.
 *
 * @return The sensor data in a log format.
 */
char *CURSRData::getLogSensorData()
{
  char *logData = new char[100];
  SensorData sense = this->getRawSensorData();
  sprintf(logData, "%f,%f,%f,%d,%d,%f,%f,%f,%f,%f,%f", sense.temperature, sense.pressure, sense.altitude, sense.latitude, sense.longitude, sense.accelerationX, sense.accelerationY, sense.accelerationZ, sense.gyroscopeX, sense.gyroscopeY, sense.gyroscopeZ);
  return logData;
}

/**
 * @brief Returns the Kalman filtered data in a log format.
 *
 * This function returns the Kalman filtered data in a log format.
 *
 * @return The Kalman filtered data in a log format.
 */
char *CURSRData::getLogKalmanData()
{
  char *logData = new char[100];
  SensorData sense = this->getKalmanFilteredData();
  sprintf(logData, "%f,%f,%f,%d,%d,%f,%f,%f,%f,%f,%f", sense.temperature, sense.pressure, sense.altitude, sense.latitude, sense.longitude, sense.accelerationX, sense.accelerationY, sense.accelerationZ, sense.gyroscopeX, sense.gyroscopeY, sense.gyroscopeZ);
  return logData;
}

/**
 * @brief Decodes the sensor data.
 *
 * This function decodes the sensor data from a byte array.
 *
 * @param encodedData The encoded sensor data.
 */
void CURSRData::decodeSensorData(char *encodedData)
{
  memcpy(&sensorData, encodedData, sizeof(SensorData));
}

/**
 * @brief Sets the temperature.
 *
 * This function sets the temperature in the sensor data.
 *
 * @param temperature The temperature to be set.
 */
void CURSRData::setTemperature(float temperature)
{
  sensorData.temperature = temperature;
  kalmanFilterMetrics.setTemperature(temperature);
}

/**
 * @brief Sets the pressure.
 *
 * This function sets the pressure in the sensor data.
 *
 * @param pressure The pressure to be set.
 */
void CURSRData::setPressure(float pressure)
{
  sensorData.pressure = pressure;
  kalmanFilterMetrics.setPressure(pressure);
}

/**
 * @brief Sets the altitude.
 *
 * This function sets the altitude in the sensor data.
 *
 * @param altitude The altitude to be set.
 */
void CURSRData::setAltitude(float altitude)
{
  sensorData.altitude = altitude;
  kalmanFilterMetrics.setAltitude(altitude);
}

/**
 * @brief Sets the latitude.
 *
 * This function sets the latitude in the sensor data.
 *
 * @param latitude The latitude to be set.
 */
void CURSRData::setLatitude(int32_t latitude)
{
  sensorData.latitude = latitude;
}

/**
 * @brief Sets the longitude.
 *
 * This function sets the longitude in the sensor data.
 *
 * @param longitude The longitude to be set.
 */

void CURSRData::setLongitude(int32_t longitude)
{
  sensorData.longitude = longitude;
}

/**
 * @brief Sets the acceleration in the X-axis.
 *
 * This function sets the acceleration in the X-axis in the sensor data.
 *
 * @param accelerationX The acceleration in the X-axis to be set.
 */
void CURSRData::setAccelerationX(float accelerationX)
{
  sensorData.accelerationX = accelerationX;
  kalmanFilterMetrics.setAcceleration(accelerationX, sensorData.accelerationY, sensorData.accelerationZ);
}

/**
 * @brief Sets the acceleration in the Y-axis.
 *
 * This function sets the acceleration in the Y-axis in the sensor data.
 *
 * @param accelerationY The acceleration in the Y-axis to be set.
 */
void CURSRData::setAccelerationY(float accelerationY)
{
  sensorData.accelerationY = accelerationY;
  kalmanFilterMetrics.setAcceleration(sensorData.accelerationX, accelerationY, sensorData.accelerationZ);
}

/**
 * @brief Sets the acceleration in the Z-axis.
 *
 * This function sets the acceleration in the Z-axis in the sensor data.
 *
 * @param accelerationZ The acceleration in the Z-axis to be set.
 */
void CURSRData::setAccelerationZ(float accelerationZ)
{
  sensorData.accelerationZ = accelerationZ;
  kalmanFilterMetrics.setAcceleration(sensorData.accelerationX, sensorData.accelerationY, accelerationZ);
}

/**
 * @brief Sets the gyroscope in the X-axis.
 *
 * This function sets the gyroscope in the X-axis in the sensor data.
 *
 * @param gyroscopeX The gyroscope in the X-axis to be set.
 */
void CURSRData::setGyroscopeX(float gyroscopeX)
{
  sensorData.gyroscopeX = gyroscopeX;
  kalmanFilterMetrics.setGyroscope(gyroscopeX, sensorData.gyroscopeY, sensorData.gyroscopeZ);
}

/**
 * @brief Sets the gyroscope in the Y-axis.
 *
 * This function sets the gyroscope in the Y-axis in the sensor data.
 *
 * @param gyroscopeY The gyroscope in the Y-axis to be set.
 */
void CURSRData::setGyroscopeY(float gyroscopeY)
{
  sensorData.gyroscopeY = gyroscopeY;
  kalmanFilterMetrics.setGyroscope(sensorData.gyroscopeX, gyroscopeY, sensorData.gyroscopeZ);
}

/**
 * @brief Sets the gyroscope in the Z-axis.
 *
 * This function sets the gyroscope in the Z-axis in the sensor data.
 *
 * @param gyroscopeZ The gyroscope in the Z-axis to be set.
 */
void CURSRData::setGyroscopeZ(float gyroscopeZ)
{
  sensorData.gyroscopeZ = gyroscopeZ;
  kalmanFilterMetrics.setGyroscope(sensorData.gyroscopeX, sensorData.gyroscopeY, gyroscopeZ);
}
