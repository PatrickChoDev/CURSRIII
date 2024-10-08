#include <maxm10s.hpp>

void CURSRSensor::checkMAXM10SSensor()
{
#if (MAXM10S_ENABLED)
  if (gnss.begin())
  {
    gnss.setI2COutput(COM_TYPE_UBX);
    gnss.setNavigationFrequency(20);
    gnss.saveConfigSelective(VAL_CFG_SUBSEC_IOPORT);
    this->maxm10sAvailable = true;
    log("MAXM10S sensor found and setup");
  }
  else
  {
    log("MAXM10S sensor not found");
    this->maxm10sAvailable = false;
  }
#endif
}