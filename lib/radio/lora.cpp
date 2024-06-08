#include <radio.hpp>
#include <lora.hpp>

void CURSRRadio::setupTx()
{
#if (LORA_ENABLED)
  LoRaSPI->setDataMode(SPI_MODE0);
  LoRaSPI->setFrequency(16000000);
  LoRaSPI->begin(LORA_SCK_PIN, LORA_MISO_PIN, LORA_MOSI_PIN, LORA_NSS_PIN);
  LoRa.setSPI(*LoRaSPI, 16000000U);
  while (!radioAvailable)
  {
    if (!LoRa.begin(LORA_NSS_PIN, LORA_RESET_PIN, LORA_BUSY_PIN, LORA_IRQ_PIN, -1, -1))
    {
      Serial.printf("LoRa not ready: %d\n", LoRa.getError());
    }
    else
    {
      log("LoRa ready");
      LoRa.setDio3TcxoCtrl(LORA_DIO3_VOLTAGE, LORA_DIO3_TCXO);
      LoRa.setFrequency(LORA_FREQUENCY);
      LoRa.setTxPower(LORA_POWER, SX126X_TX_POWER_SX1262);
      LoRa.setDio2RfSwitch(true);
      LoRa.setLoRaModulation(LORA_SPREADING_FACTOR, LORA_BANDWIDTH, LORA_CODING_RATE, true);
      LoRa.setLoRaPacket(LORA_HEADER_MODE, LORA_PREAMBLE_LENGTH, LORA_PAYLOAD_LENGTH, LORA_CRC);
      LoRa.setSyncWord(LORA_SYNC_WORD);
      radioAvailable = true;
      return;
    }
    delay(800); /* To not spam LoRa */
  }
#else
  log("LoRa not enabled");
  radioAvailable = false;
#endif
}

void CURSRRadio::setupRx()
{
#if (LORA_ENABLED)
  LoRaSPI->setDataMode(SPI_MODE0);
  LoRaSPI->setFrequency(16000000);
  LoRaSPI->begin(LORA_SCK_PIN, LORA_MISO_PIN, LORA_MOSI_PIN, LORA_NSS_PIN);
  LoRa.setSPI(*LoRaSPI, 16000000U);
  while (!radioAvailable)
  {
    if (!LoRa.begin(LORA_NSS_PIN, LORA_RESET_PIN, LORA_BUSY_PIN, LORA_IRQ_PIN, -1, -1))
    {
      Serial.printf("LoRa not ready: %d\n", LoRa.getError());
    }
    else
    {
      log("LoRa ready");
      LoRa.setDio3TcxoCtrl(LORA_DIO3_VOLTAGE, LORA_DIO3_TCXO);
      LoRa.setDio2RfSwitch(true);
      LoRa.setFrequency(LORA_FREQUENCY);
      LoRa.setRxGain(LORA_RX_POWER);
      LoRa.setLoRaModulation(LORA_SPREADING_FACTOR, LORA_BANDWIDTH, LORA_CODING_RATE, true);
      LoRa.setLoRaPacket(SX126X_HEADER_EXPLICIT, LORA_PREAMBLE_LENGTH, LORA_PAYLOAD_LENGTH, LORA_CRC);
      LoRa.setSyncWord(LORA_SYNC_WORD);
      radioAvailable = true;
      return;
    }
    delay(800); /* To not spam LoRa */
  }
#else
  log("LoRa not enabled");
  radioAvailable = false;
#endif
}