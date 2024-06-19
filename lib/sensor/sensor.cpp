
#include <sensor.hpp>
#include <bmp390.hpp>
#include <adxl375.hpp>
#include <maxm10s.hpp>

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
  checkBNO055Sensor();
  checkBMP390Sensor();
  checkADXLSensor();
  checkMAXM10SSensor();
  log("End sensor setup");
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
#if (SENSOR_LOG_ENABLED)
  Serial.println(message);
#endif
}

/**
 * @brief Reads the sensor value.
 *
 * This function reads the sensor value and stores it in the sensor value variables.
 */
void CURSRSensor::readSensor()
{
#if (BMP390_ENABLED)
  if (bmp390Available)
  {
    altitude = bmp.readAltitude(BMP390_SEA_LEVEL_PRESSURE);
    temperature = bmp.readTemperature();
    pressure = bmp.readPressure();
  }
#endif
  sensors_event_t event, linearAccelData, angVelData, orientationData, accelData;
#if (ADXL375_ENABLED)
  if (ADXLAvailable)
  {
    accel.getEvent(&event);
  }
#endif
#if (BNO055_ENABLED)
  if (bnoAvailable)
  {
    bno.getEvent(&linearAccelData, Adafruit_BNO055::VECTOR_LINEARACCEL);
    bno.getEvent(&angVelData, Adafruit_BNO055::VECTOR_GYROSCOPE);
    bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
    bno.getEvent(&accelData, Adafruit_BNO055::VECTOR_ACCELEROMETER);
  }
#endif
#if (ADXL375_ENABLED || BNO055_ENABLED)
  if (ADXLAvailable && bnoAvailable)
  {
    gyroX = (angVelData.gyro.x + event.gyro.x) / 2;
    gyroY = (angVelData.gyro.y + event.gyro.y) / 2;
    gyroZ = (angVelData.gyro.z + event.gyro.z) / 2;
    accelX = (event.acceleration.x + accelData.acceleration.x) / 2;
    accelY = (event.acceleration.y + accelData.acceleration.y) / 2;
    accelZ = (event.acceleration.z + accelData.acceleration.z) / 2;
  }
  else if (ADXLAvailable)
  {
    accelX = event.acceleration.x;
    accelY = event.acceleration.y;
    accelZ = event.acceleration.z;
    gyroX = event.gyro.x;
    gyroY = event.gyro.y;
    gyroZ = event.gyro.z;
  }
#if (BNO055_ENABLED)
  else if (bnoAvailable)
  {
    accelX = accelData.acceleration.x;
    accelY = accelData.acceleration.y;
    accelZ = accelData.acceleration.z;
    gyroX = angVelData.gyro.x;
    gyroY = angVelData.gyro.y;
    gyroZ = angVelData.gyro.z;
  }
#endif
#endif
#if (MAXM10S_ENABLED)
  if (!maxm10sAvailable)
    checkMAXM10SSensor();
  if (gnss.getPVT(5))
  {
    timestamp = gnss.getUnixEpoch(5);
    latitude = gnss.getLatitude(5);
    longitude = gnss.getLongitude(5);
    altitudeGPS = gnss.getAltitudeMSL(10);
  }
#endif
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
  data->setAltitude(altitude);
  data->setAccelerationX(accelX);
  data->setAccelerationY(accelY);
  data->setAccelerationZ(accelZ);
  data->setGyroscopeX(gyroX);
  data->setGyroscopeY(gyroY);
  data->setGyroscopeZ(gyroZ);
  data->setPressure(pressure);
  data->setTemperature(temperature);
  data->setTimestamp(timestamp);
  data->setLatitude(latitude);
  data->setLongitude(longitude);
  data->setAltitudeGPS(altitudeGPS);
}

void CURSRSensor::setBMP390Available(bool available)
{
  bmp390Available = available;
}