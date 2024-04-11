
#include <sensor.hpp>
#include <bmp390.hpp>

/**
 * @brief Sets up the sensor.
 *
 * This function initializes the sensor by starting the Wire communication and searching for the sensor's I2C address.
 * Sensor's I2C address will define in header files and can be changed in the future. ( SENSORNAME_ADDRESS )
 * Sensor can be disabled by setting SENSORNAME_ENABLED to 0 in the header file.
 *
 * @note This function assumes that the Wire library and Serial communication have been properly initialized.
 */
void CURSRSensor::setup()
{
  log("Start sensor setup");
  checkBMP390Sensor();
}

/**
 * @brief Logs a message to the Serial.
 *
 * This function logs a message to the Serial if the SENSOR_LOG_ENABLED is enabled.
 *
 * @param message The message to be logged.
 */
void CURSRSensor::log(String message)
{
  if (SENSOR_LOG_ENABLED)
  {
    Serial.println(message);
  }
}

/**
 * @brief Reads the sensor value.
 *
 * This function reads the sensor value and stores it in the sensorValue variable.
 */
void CURSRSensor::readSensor()
{
  if (bmp390Available)
  {
    sensorValue = bmp.readPressure();
  }
  else
  {
    sensorValue = 0;
  }
  log("Sensor value: " + String(sensorValue));
}

/**
 * @brief Returns the sensor value.
 *
 * This function returns the sensor value.
 *
 * @return The sensor value.
 */
long CURSRSensor::getSensorValue()
{
  return sensorValue;
}

void CURSRSensor::setBMP390Available(bool available)
{
  bmp390Available = available;
}