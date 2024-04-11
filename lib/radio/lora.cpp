#include <radio.hpp>
#include <lora.hpp>

void CURSRRadio::loraSetup()
{
  if (LORA_ENABLED)
  {
    SPIClass LoRaSPI(HSPI);
    LoRaSPI.begin(LORA_SCK_PIN, LORA_MISO_PIN, LORA_MOSI_PIN, LORA_NSS_PIN);
    LoRa.setSPI(LoRaSPI);
    while (!radioAvailable)
    {
      if (!LoRa.begin(LORA_NSS_PIN, LORA_RESET_PIN, LORA_BUSY_PIN, LORA_IRQ_PIN, -1, -1))
      {
        log("LoRa not ready: " + String(LoRa.getError()));
      }
      else
      {
        log("LoRa ready");
        LoRa.setFrequency(LORA_FREQUENCY);
        LoRa.setTxPower(LORA_POWER);
        LoRa.setBandwidth(LORA_BANDWIDTH);
        LoRa.setSpreadingFactor(LORA_SPREADING_FACTOR);
        LoRa.setCodeRate(LORA_CODING_RATE);
        LoRa.setPreambleLength(LORA_PREAMBLE_LENGTH);
        LoRa.setSyncWord(LORA_SYNC_WORD);
        LoRa.setDio3TcxoCtrl(LORA_DIO3_VOLTAGE, LORA_DIO3_TCXO);
        LoRa.request(LORA_LISTEN_MODE);
        radioAvailable = true;
        return;
      }
      delay(200); /* To not spam LoRa */
    }
  }
  else
  {
    radioAvailable = false;
  }
}
