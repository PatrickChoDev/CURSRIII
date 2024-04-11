#include <sensor.hpp>
#include <bmp390.hpp>

void CURSRSensor::checkBMP390Sensor()
{
  if (BMP390_ENABLED)
  {
    if (bmp.begin_I2C())
    {
      log("BMP390 sensor found");
      this->bmp390Available = true;
      bmp.setTemperatureOversampling(BMP390_TEMP_OVERSAMPLING_MODE);
      bmp.setPressureOversampling(BMP390_PRESSURE_OVERSAMPLING_MODE);
      bmp.setIIRFilterCoeff(BMP390_IIR_FILTER_COEFF);
      bmp.setOutputDataRate(BMP390_ODR);
    }
    else
    {
      log("BMP390 sensor not found");
      this->bmp390Available = false;
    }
  }
  else
  {
    log("BMP390 sensor disabled");
    this->bmp390Available = false;
  }
}