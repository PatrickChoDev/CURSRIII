#include <sensor.hpp>
#include <bmp390.hpp>

void CURSRSensor::checkBMP390Sensor()
{
#if (BMP390_ENABLED)
  if (bmp.begin_I2C())
  {
    bmp.setTemperatureOversampling(BMP390_TEMP_OVERSAMPLING_MODE);
    bmp.setPressureOversampling(BMP390_PRESSURE_OVERSAMPLING_MODE);
    bmp.setIIRFilterCoeff(BMP390_IIR_FILTER_COEFF);
    bmp.setOutputDataRate(BMP390_ODR);
    this->bmp390Available = true;
    log("BMP390 sensor found and setup");
  }
  else
  {
    log("BMP390 sensor not found");
    this->bmp390Available = false;
  }
#endif
}