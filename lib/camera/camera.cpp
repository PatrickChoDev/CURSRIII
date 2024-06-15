#include <camera.hpp>

uint8_t calcCrc(uint8_t *buf, uint8_t numBytes)
{
  uint8_t crc = 0;
  for (uint8_t i = 0; i < numBytes; i++)
    crc = crc8_calc(crc, *(buf + i), 0xd5);
  return crc;
}

uint8_t crc8_calc(uint8_t crc, unsigned char a, uint8_t poly)
{
  crc ^= a;
  for (int ii = 0; ii < 8; ++ii)
  {
    if (crc & 0x80)
      crc = (crc << 1) ^ poly;
    else
      crc = crc << 1;
  }
  return crc;
}

void getDeviceInfo()
{
  uint8_t crc = 0;
  uint8_t exp_crc = 0;
  uint8_t txBuf[3] = {0};
  txBuf[0] = 0xCC;
  txBuf[1] = 0x00;
  txBuf[2] = calcCrc(txBuf, 2);
  Serial1.write(txBuf, 3);
  Serial.printf("%02X %02X %02X \n", txBuf[0], txBuf[1], txBuf[2]);
  Serial.println("Sent device info command");
  uint8_t rxByte[5] = {0};
  uint8_t idx = 0;
  while (Serial1.available() > 0)
  {
    uint8_t ch = Serial1.read();
    Serial.printf("ch = %02X\n", ch);
    if (idx == 0)
    {
      if (ch == 0xCC)
      {
        rxByte[idx] = ch;
        idx++;
      }
    }
    else
    {
      rxByte[idx] = ch;
      idx++;
      if (idx == 5)
      {
        idx = 0;
        crc = calcCrc(rxByte, 4);
        exp_crc = rxByte[4];
        if (crc == exp_crc)
        {
          Serial.printf("Camera version: %d\n", rxByte[1]);
        }
        else
          Serial.println("CRC error");
      }
    }
  }
}

void sendStartCommand()
{
  uint8_t txBuf[4] = {0xCC, 0x01, 0x03, 0x00};
  txBuf[3] = calcCrc(txBuf, 3);
  Serial1.write(txBuf, 4);
  Serial.println("Sent Start Recording command");
}

void sendStopCommand()
{
  uint8_t txBuf[4] = {0xCC, 0x01, 0x04, 0x00};
  txBuf[3] = calcCrc(txBuf, 3);
  Serial1.write(txBuf, 4);
  Serial.println("Sent stop recording command");
}

void sendSwitchCommand()
{
  uint8_t txBuf[4] = {0xCC, 0x01, 0x02, 0x00};
  txBuf[3] = calcCrc(txBuf, 3);
  Serial1.write(txBuf, 4);
  Serial.println("Sent switch command");
}

void pressKeyCommand(uint8_t key)
{
  uint8_t txBuf[4] = {0xCC, 0x02, key, 0x00};
  txBuf[3] = calcCrc(txBuf, 3);
  Serial1.write(txBuf, 4);
  Serial.flush();
  delay(100);
  txBuf[1] = 0x03;
  txBuf[3] = calcCrc(txBuf, 3);
  Serial1.write(txBuf, 4);
  Serial.println("Press key command");
}

void sendPowerButton()
{
  uint8_t txBuf[4] = {0xCC, 0x01, 0x01, 0x00};
  txBuf[3] = calcCrc(txBuf, 3);
  Serial1.write(txBuf, 4);
  Serial.println("Sent power button command");
}

void sendOSDCommand()
{
  uint8_t txBuf[4] = {0xCC, 0x01, 0x00, 0x00};
  txBuf[3] = calcCrc(txBuf, 3);
  Serial1.write(txBuf, 4);
  Serial.println("Sent OSD command");
}

void switchUARTTo(uint8_t mode)
{
  if (mode == 1)
  {
    digitalWrite(33, LOW);
    digitalWrite(25, LOW);
  }
  else if (mode == 2)
  {
    digitalWrite(33, HIGH);
    digitalWrite(25, LOW);
  }
  else if (mode == 3)
  {
    digitalWrite(33, LOW);
    digitalWrite(25, HIGH);
  }
  else if (mode == 4)
  {
    digitalWrite(33, HIGH);
    digitalWrite(25, HIGH);
  }
  Serial.printf("Switched to UART %d\n", mode);
}

void switchVideoTo(uint8_t mode)
{
  if (mode == 1)
  {
    digitalWrite(26, LOW);
    digitalWrite(27, LOW);
  }
  else if (mode == 2)
  {
    digitalWrite(26, HIGH);
    digitalWrite(27, LOW);
  }
  else if (mode == 3)
  {
    digitalWrite(26, LOW);
    digitalWrite(27, HIGH);
  }
  else if (mode == 4)
  {
    digitalWrite(26, HIGH);
    digitalWrite(27, HIGH);
  }
  Serial.printf("Switched to video channel %d\n", mode);
}