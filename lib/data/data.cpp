#include <data.hpp>

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
SensorData CURSRData::getSensorData()
{
  return sensorData;
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
  memcpy(encodedData, &sensorData, sizeof(SensorData));
  return encodedData;
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
}
