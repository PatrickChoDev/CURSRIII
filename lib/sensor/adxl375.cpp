#include <adxl375.hpp>

void CURSRSensor::checkADXLSensor()
{
#if (ADXL375_ENABLED)

  if (accel.begin())
  {
    accel.printSensorDetails();
    accel.setTrimOffsets(0, 0, 0);

    Serial.println("Hold accelerometer flat to set offsets to 0, 0, and -1g...");
    delay(1000);
    int16_t x, y, z;
    x = accel.getX();
    y = accel.getY();
    z = accel.getZ();
    Serial.print("Raw X: ");
    Serial.print(x);
    Serial.print("  ");
    Serial.print("Y: ");
    Serial.print(y);
    Serial.print("  ");
    Serial.print("Z: ");
    Serial.print(z);
    Serial.print("  ");
    Serial.println(" counts");

    // the trim offsets are in 'multiples' of 4, we want to round, so we add 2
    accel.setTrimOffsets(-(x + 2) / 4,
                         -(y + 2) / 4,
                         -(z - 20 + 2) / 4); // Z should be '20' at 1g (49mg per bit)

    int8_t x_offset, y_offset, z_offset;
    accel.getTrimOffsets(&x_offset, &y_offset, &z_offset);
    Serial.print("Current trim offsets: ");
    Serial.print(x_offset);
    Serial.print(", ");
    Serial.print(y_offset);
    Serial.print(", ");
    Serial.println(z_offset);

    Serial.println();
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