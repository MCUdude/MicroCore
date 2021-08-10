#include <TinyWire.h>

const uint8_t TinyWire::SCL = 3;
const uint8_t TinyWire::SDA = 4;
const uint32_t TinyWire::FREQ = 100000;

const uint8_t eeprom_address = 0x50;

void setup()
{
  Serial.begin();
}

void loop()
{
  // Fill the first 256 bytes with data, from 255 to 0
  for(int16_t i = 0; i < 256; i++)
  {
    Wire.beginTransmission(eeprom_address);
    Wire.write(0x00); // High address byte
    Wire.write(i);    // Low address byte
    Wire.write(0xFF - i);
    Wire.endTransmission();
    delay(3);
  }
  delay(500);

  Serial.println(F("Data written to EEPROM. Content:"));

  Wire.beginTransmission(eeprom_address);
  Wire.write(0x00); // High address byte
  Wire.write(0x00); // Low address byte
  Wire.endTransmission();

  Wire.requestFrom(eeprom_address, 256);
  for(int16_t i = 0; i < 256; i++)
  {
    uint8_t i2cdata = Wire.read();
    Serial.print(F("i: 0x"));
    Serial.print(i, HEX);
    Serial.print(F(" Data: 0x"));
    Serial.println(i2cdata, HEX);
  }
  Wire.endTransmission();

  while(1);
}
