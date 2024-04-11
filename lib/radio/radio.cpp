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
    LoRa.write(data, sizeof(data));
    LoRa.endPacket();
    log("Data sent: " + String(data));
    LoRa.wait();
    log("Transmission completed: " + String(LoRa.transmitTime()) + "ms");
  }
  else
  {
    log("Radio not available");
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