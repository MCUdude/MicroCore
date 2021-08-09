#include <TinyWire.h>

const uint8_t TinyWire::SCL = 3;
const uint8_t TinyWire::SDA = 4;
const uint32_t TinyWire::FREQ = MAX_SPEED;

void setup()
{
  Serial.begin();
}


void loop()
{
  uint8_t error, address;
  int nDevices;

  Serial.println(F("Scanning..."));

  nDevices = 0;
  for(address = 1; address < 127; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print(F("I2C device found at address 0x"));
      if (address < 16)
        Serial.write('0');
      Serial.println(address,HEX);
      nDevices++;
    }
  }
  if (nDevices == 0)
    Serial.println(F("No I2C devices found\n"));
  else
    Serial.println(F("Done!\n"));

  delay(5000);           // Wait 5 seconds for next scan
}
