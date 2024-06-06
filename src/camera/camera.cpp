#include <camera.h>

void setup()
{
  Serial1.begin(115200, SERIAL_8N1, 35, 32);
  delay(200);
  while (Serial1.available() > 0)
    Serial1.read();
  Serial.begin(115200);
  getDeviceInfo();
  delay(1000);
}

void loop()
{
  if (Serial1.available() > 0)
  {
    uint8_t ch = Serial1.read();
    Serial.printf("ch = %02X\n\r", ch);
  }
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
  }
}