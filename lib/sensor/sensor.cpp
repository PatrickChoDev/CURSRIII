
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
 * This function reads the sensor value and stores it in the sensor value variables.
 */
void CURSRSensor::readSensor()
{
  if (bmp390Available)
  {
    altitude = bmp.readAltitude(BMP390_SEE_LEVEL_PRESSURE);
    temperature = bmp.readTemperature();
    pressure = bmp.readPressure();
  }
  else
  {
    altitude = 0;
    temperature = 0;
    pressure = 0;
  }
  // log("Sensor value: " + String(altitude) + "m, " + String(temperature) + "C, " + String(pressure) + "Pa");
}

/**
 * @brief Gets the sensor value.
 *
 * This function gets the sensor value and stores it in the data object.
 *
 * @param data The data object to store the sensor value.
 */
void CURSRSensor::getSensorValue(CURSRData *data)
{
  data->setPressure(pressure);
  data->setTemperature(temperature);
}

void CURSRSensor::setBMP390Available(bool available)
{
  bmp390Available = available;
}