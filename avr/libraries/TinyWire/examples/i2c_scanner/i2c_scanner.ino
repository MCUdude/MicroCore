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
| In this example we scan the i2c bus for attached devices and prints   |
| out the result on the serial monitor.                                 |
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

void setup()
{
  // No baudrate to select. See header for details
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
