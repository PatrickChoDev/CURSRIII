#include <adxl375.hpp>

void CURSRSensor::checkADXLSensor()
{
#if (ADXL375_ENABLED)

  if (accel.begin())
  {
    accel.printSensorDetails();
    accel.setTrimOffsets(0, 0, 0);

    Serial.println("Hold accelerometer flat to set offsets to 0, 0, and -1g...");
    delay(200);
    int16_t x, y, z;
    x = accel.getX();
    y = accel.getY();
    z = accel.getZ();
    accel.setTrimOffsets(-(x + 2) / 4,
                         -(y + 2) / 4,
                         -(z - 20 + 2) / 4); // Z should be '20' at 1g (49mg per bit)
    this->ADXLAvailable = true;
    log("ADXL sensor found and setup");
  }
  else
  {
    log("ADXL sensor not found");
    this->ADXLAvailable = false;
  }
#endif
}