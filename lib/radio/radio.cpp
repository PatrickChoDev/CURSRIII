#include <radio.hpp>

void CURSRRadio::log(String message)
{
  if (RADIO_LOG_ENABLED)
  {
    Serial.println(message);
  }
}

void CURSRRadio::send(char *data)
{
  if (radioAvailable)
  {
    LoRa.beginPacket();
    LoRa.write(data, strlen(data));
    LoRa.endPacket();
    LoRa.wait();
  }
}

void CURSRRadio::receive()
{
  if (radioAvailable)
  {
    msgLen = LoRa.available();
    if (msgLen)
    {
      LoRa.read(message, msgLen);
    }
  }
}

char *CURSRRadio::getData()
{
  return message;
}