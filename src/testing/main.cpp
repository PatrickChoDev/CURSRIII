#include <Arduino.h>
#include <Wire.h>

void setup()
{
  Serial.begin(115200);
  Serial1.setPins(23, 22);
  Wire.end();
  pinMode(23, INPUT);
  pinMode(22, OUTPUT);
  Serial1.begin(115200);
}

void loop()
{
  if (Serial1.available() > 0)
  {
    uint8_t ch = Serial1.read();
    Serial.printf("ch = %02X\n\r", ch);
  }
}