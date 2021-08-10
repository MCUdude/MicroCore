/***********************************************************************|
| TinyWSoftware i2c master library for ATtiny13                         |
|                                                                       |
| i2c_scanner.ino                                                       |
|                                                                       |
| A light-weight library for interfacing with i2c slave devices.        |
| Requires extenal pullup resistors on the SDA and SCL line!            |
| Developed by MCUdude, based on nerdralph's PicoI2C library            |
| https://github.com/MCUdude/MicroCore                                  |
|                                                                       |
| In this example we write to and read data from a generic i2c EEPROM   |
| chip such as 24C256.                                                  |
|                                                                       |
| SERIAL REMINDER                                                       |
| The baud rate is ignored on the ATtiny13 due to using a simplified    |
| serial. The baud rate used is dependent on the processor speed.       |
| Note that you can specify a custom baud rate if the following ones    |
| does not fit your application.                                        |
|                                                                       |
| THESE CLOCKS USES 115200 BAUD:       THIS CLOCK USES 57600 BAUD:      |
| 20 MHz, 16 MHz, 9.6 MHz, 8 MHz       4.8 MHz                          |
| THESE CLOCKS USES 19200 BAUD:        THIS CLOCK USES 9600 BAUD:       |
| 1.2 MHz, 1 MHz                       600 KHz                          |
|                                                                       |
| If you get garbage output:                                            |
|  1. Check baud rate as above                                          |
|  2. Check if you have anything else connected to TX/RX like an LED    |
|  3. Check OSCCAL (see MicroCore OSCCAL tuner example)                 |
|***********************************************************************/

#include <TinyWire.h>

// We have to define which pins to use for SDA and SCL
const uint8_t TinyWire::SCL = 3;
const uint8_t TinyWire::SDA = 4;

// Sets the i2c clock speed in [Hz]
// Use MAX_SPEED for the highest possible clock speed
const uint32_t TinyWire::FREQ = 100000;

// 7-bit i2c address for the attached EEPROM
const uint8_t eeprom_address = 0x50;

void setup()
{
  // No baudrate to select. See header for details
  Serial.begin();
}

void loop()
{
  // Fill the first 256 bytes with data, from 255 to 0
  for(uint16_t i = 0; i < 256; i++)
  {
    Wire.beginTransmission(eeprom_address);
    Wire.write(0x00); // High address byte
    Wire.write(i);    // Low address byte
    Wire.write(0xFF - i);
    Wire.endTransmission();
    delayMicroseconds(3000); // EEPROM is SLOW!
  }

  Serial.print(F("Data written to EEPROM. Content:\n"));

  // Initialize read from EEPROM memory address 0
  Wire.beginTransmission(eeprom_address);
  Wire.write(0x00); // High address byte
  Wire.write(0x00); // Low address byte
  Wire.endTransmission();

  // Read 256 bytes of data
  Wire.requestFrom(eeprom_address, 256);
  for(uint16_t i = 0; i < 256; i++)
  {
    uint8_t i2cdata = Wire.read();
    Serial.print(F("i: 0x"));
    Serial.print(i, HEX);
    Serial.print(F(" Data: 0x"));
    Serial.print(i2cdata, HEX);
    Serial.write('\n');
  }
  Wire.endTransmission();

  while(1);
}