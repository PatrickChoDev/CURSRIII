#include <SoftwareSerial.h>

SoftwareSerial cameraSerial;

uint8_t crc8_dvb_s2(uint8_t crc, unsigned char a)
{
  crc ^= a;
  for (int ii = 0; ii < 8; ++ii)
  {
    if (crc & 0x80)
    {
      crc = (crc << 1) ^ 0xD5;
    }
    else
    {
      crc = crc << 1;
    }
  }
  return crc;
}

static uint8_t crc8HighFirst(uint8_t *ptr, uint8_t len)
{
  uint8_t crc = 0x00;
  while (len--)
  {
    crc ^= *ptr++;
    for (unsigned i = 8; i > 0; --i)
    {
      if (crc & 0x80)
        crc = (crc << 1) ^ 0x31;
      else
        crc = (crc << 1);
    }
  }
  return (crc);
}

void setup()
{
  Serial.begin(115200);
  // Serial1.begin(115200, SWSERIAL_8N1, 35, 32);
  cameraSerial.begin(115200, SWSERIAL_8N2, 35, 32, false, 256);
  uint8_t crc = 0;
  uint8_t info_command[3] = {0xCC, 0x00, 0x00};
  crc = crc8_dvb_s2(crc, info_command[0]);
  crc = crc8_dvb_s2(crc, info_command[1]);
  info_command[2] = crc;
  cameraSerial.flush();
  cameraSerial.clearWriteError();
  cameraSerial.write(info_command, 3);
  cameraSerial.flush();
  Serial.print("Command: ");
  for (int i = 0; i < sizeof(info_command); i++)
  {
    Serial.printf("%02X ", info_command[i]);
  }
  Serial.println();
  Serial.print("Response:");
  while (cameraSerial.available() < 5)
  {
    // Wait until 5 bytes are available
  }
  for (int i = 0; i < 5; i++)
  {
    Serial.printf("%02X ", cameraSerial.read());
  }
  while (cameraSerial.available() > 0)
  {
    Serial.printf("%02X ", cameraSerial.read());
  }
  Serial.println();

  uint8_t start_command[4] = {0xCC, 0x01, 0x03, 0x00};
  crc = crc8_dvb_s2(0, start_command[0]);
  crc = crc8_dvb_s2(crc, start_command[1]);
  crc = crc8_dvb_s2(crc, start_command[2]);
  start_command[3] = crc;
  cameraSerial.flush();
  cameraSerial.clearWriteError();
  cameraSerial.write(start_command, 4);
  cameraSerial.flush();
  Serial.print("Command: ");
  for (int i = 0; i < sizeof(start_command); i++)
  {
    Serial.printf("%02X ", start_command[i]);
  }
  Serial.println();
  Serial.print("Response:");
  while (cameraSerial.available() < 1)
  {
    // Wait until 5 bytes are available
  }
  for (int i = 0; i < 1; i++)
  {
    Serial.printf("%02X ", cameraSerial.read());
  }
  while (cameraSerial.available() > 0)
  {
    Serial.printf("%02X ", cameraSerial.read());
  }
  Serial.println();
}

void loop()
{
  delay(1);
}