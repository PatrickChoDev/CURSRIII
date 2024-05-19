#include <bno055.hpp>

/**
 * @brief Initializes the BNO055 sensor.
 *
 * This function initializes the BNO055 sensor.
 */
void CURSRSensor::checkBNO055Sensor()
{
#if (BNO055_ENABLED)
  if (!bno.begin())
  {
    log("BNO055 sensor not found");
    bnoAvailable = false;
  }
  else
  {
    log("BNO055 sensor found");
    bnoAvailable = true;
  }
#endif
}