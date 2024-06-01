#include <SX126x.h>
#include <SPI.h>

#ifndef LORA_NSS_PIN
#define LORA_NSS_PIN 5
#endif

#ifndef LORA_RESET_PIN
#define LORA_RESET_PIN 16
#endif

#ifndef LORA_BUSY_PIN
#define LORA_BUSY_PIN 17
#endif

#ifndef LORA_IRQ_PIN
#define LORA_IRQ_PIN -1
#endif

#ifndef LORA_SCK
#define LORA_SCK_PIN 19
#endif

#ifndef LORA_MISO_PIN
#define LORA_MISO_PIN 18
#endif

#ifndef LORA_MOSI_PIN
#define LORA_MOSI_PIN 23
#endif

SX126x LoRa;

// Message to transmit
char message[] = "HeLoRa World!";
uint8_t nBytes = sizeof(message);
uint8_t counter = 0;

void setup()
{

  // Begin serial communication
  Serial.begin(115200);
  SPIClass *LoRaSPI = new SPIClass(VSPI);
  LoRaSPI->setDataMode(SPI_MODE0);
  LoRaSPI->setFrequency(8000000);
  LoRaSPI->begin(LORA_SCK_PIN, LORA_MISO_PIN, LORA_MOSI_PIN, LORA_NSS_PIN);
  LoRa.setSPI(*LoRaSPI, 8000000U);
  // Uncomment below to use non default SPI port
  // SPIClass SPI_2(PB15, PB14, PB13);
  // LoRa.setSPI(SPI_2, 16000000);
  Serial.println("Begin LoRa radio");
  // for ESP32 30 pin
  int8_t nssPin = 5, resetPin = 34, busyPin = 35, irqPin = -1, txenPin = -1, rxenPin = -1;
  if (!LoRa.begin(LORA_NSS_PIN, LORA_RESET_PIN, LORA_BUSY_PIN, LORA_IRQ_PIN, -1, -1))
  {
    Serial.println("Something wrong, can't begin LoRa radio");
    while (1)
      ;
  }

  // Optionally configure TCXO or XTAL used in RF module
  // Different RF module can have different clock, so make sure clock source is configured correctly
  // uncomment code below to use TCXO
  Serial.println("Set RF module to use TCXO as clock reference");
  uint8_t dio3Voltage = SX126X_DIO3_OUTPUT_1_8;
  uint32_t tcxoDelay = SX126X_TCXO_DELAY_1;
  LoRa.setDio3TcxoCtrl(dio3Voltage, tcxoDelay);
  // uncomment code below to use XTAL
  // uint8_t xtalA = 0x12;
  // uint8_t xtalB = 0x12;
  // Serial.println("Set RF module to use XTAL as clock reference");
  // LoRa.setXtalCap(xtalA, xtalB);

  // Optionally configure DIO2 as RF switch control
  // This is usually used for a LoRa module without TXEN and RXEN pins
  // LoRa.setDio2RfSwitch(true);

  // Set frequency to 915 Mhz
  Serial.println("Set frequency to 915 Mhz");
  LoRa.setFrequency(915000000);

  // Set TX power, default power for SX1262 and SX1268 are +22 dBm and for SX1261 is +14 dBm
  // This function will set PA config with optimal setting for requested TX power
  Serial.println("Set TX power to +17 dBm");
  LoRa.setTxPower(17, SX126X_TX_POWER_SX1262); // TX power +17 dBm for SX1262

  // Configure modulation parameter including spreading factor (SF), bandwidth (BW), and coding rate (CR)
  // Receiver must have same SF and BW setting with transmitter to be able to receive LoRa packet
  Serial.println("Set modulation parameters:\n\tSpreading factor = 7\n\tBandwidth = 125 kHz\n\tCoding rate = 4/5");
  uint8_t sf = 7;       // LoRa spreading factor: 7
  uint32_t bw = 125000; // Bandwidth: 125 kHz
  uint8_t cr = 5;       // Coding rate: 4/5
  bool ldro = 1;
  LoRa.setLoRaModulation(sf, bw, cr, ldro);

  Serial.println("Set packet parameters:\n\tExplicit header type\n\tPreamble length = 12\n\tPayload Length = 15\n\tCRC on");
  uint8_t headerType = SX126X_HEADER_EXPLICIT; // Explicit header mode
  uint16_t preambleLength = 4;                 // Set preamble length to 12
  uint8_t payloadLength = 20;                  // Initialize payloadLength to 15
  bool crcType = true;                         // Set CRC enable
  LoRa.setLoRaPacket(headerType, preambleLength, payloadLength, crcType);

  LoRa.setSyncWord(0x1424);

  Serial.println("\n-- LORA TRANSMITTER --\n");
}

void loop()
{

  // Transmit message and counter
  // write() method must be placed between beginPacket() and endPacket()
  LoRa.beginPacket();
  LoRa.write(message, nBytes);
  LoRa.write(counter);
  LoRa.endPacket();

  // Print message and counter in serial
  Serial.print(message);
  Serial.print("  ");
  Serial.println(counter++);

  // Wait until modulation process for transmitting packet finish
  LoRa.wait();

  // Print transmit time
  Serial.print("Transmit time: ");
  Serial.print(LoRa.transmitTime());
  Serial.println(" ms");
  Serial.println();

  // Don't load RF module with continous transmit
  delay(5000);
}