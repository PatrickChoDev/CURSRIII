#include <Arduino.h>

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

uint8_t calcCrc(uint8_t *buf, uint8_t numBytes)
{
  uint8_t crc = 0;
  for (uint8_t i = 0; i < numBytes; i++)
    crc = crc8_calc(crc, *(buf + i), 0xd5);
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
  Serial.printf("%02X %02X %02X \n\r", txBuf[0], txBuf[1], txBuf[2]);
  Serial.println("Sent device info command");
  uint8_t rxByte[5] = {0};
  uint8_t idx = 0;
  while (Serial1.available() > 0)
  {
    uint8_t ch = Serial1.read();
    Serial.printf("ch = %02X\n\r", ch);
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
          Serial.printf("Camera version: %d\n\r", rxByte[1]);
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
  Serial.printf("Switched to UART %d\n\r", mode);
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
  Serial.printf("Switched to video channel %d\n\r", mode);
}

void setup()
{
  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, 35, 32);
  while (Serial1.available() > 0)
    Serial1.read();
  Serial1.flush();
  delay(1000);
  pinMode(33, OUTPUT);
  pinMode(25, OUTPUT);
  pinMode(26, OUTPUT);
  pinMode(27, OUTPUT);
  delay(200);
  getDeviceInfo();
}

void loop()
{
  while (Serial.available() > 0)
  {
    uint8_t ch = Serial.read();
    if (ch == 's')
      sendStartCommand();
    else if (ch == 'p')
      sendPowerButton();
    else if (ch == 'o')
      sendOSDCommand();
    else if (ch == 't')
      sendStopCommand();
    else if (ch == 'w')
      sendSwitchCommand();
    else if (ch == 'd')
      getDeviceInfo();
    else if (ch == 'L')
      pressKeyCommand(0x02);
    else if (ch == 'R')
      pressKeyCommand(0x03);
    else if (ch == 'U')
      pressKeyCommand(0x04);
    else if (ch == 'D')
      pressKeyCommand(0x05);
    else if (ch == 'S')
      pressKeyCommand(0x01);
    else if (ch == '1')
      switchUARTTo(1);
    else if (ch == '2')
      switchUARTTo(2);
    else if (ch == '3')
      switchUARTTo(3);
    else if (ch == '4')
      switchUARTTo(4);
    else if (ch == '5')
      switchVideoTo(1);
    else if (ch == '6')
      switchVideoTo(2);
    else if (ch == '7')
      switchVideoTo(3);
    else if (ch == '8')
      switchVideoTo(4);
  }
}