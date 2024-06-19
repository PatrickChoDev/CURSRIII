#include <Arduino.h>
#include <Wire.h>

void setup()
{
  Serial.begin(115200);
  Wire.end();
  pinMode(21, OUTPUT);
  pinMode(22, OUTPUT);
}

void loop()
{
  if (Serial.available())
  {
    char ch = Serial.read();
    Serial.printf("ch = %02X\n\r", ch);
    if (ch == 'm')
      digitalWrite(22, HIGH);
    else if (ch == 'M')
      digitalWrite(22, LOW);
    else if (ch == 'd')
      digitalWrite(21, HIGH);
    else if (ch == 'D')
      digitalWrite(21, LOW);
  }
}