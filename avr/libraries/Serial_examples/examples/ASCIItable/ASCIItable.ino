/*
  Print a table of ASCII characters to the Serial Monitor

  By J.Sleeman (sparks@gogo.co.nz)
  MicroCore support by MCUdude
  ------------------------------------------------------------------------------

  [ See diagram: https://github.com/MCUdude/MicroCore#minimal-setup ]

  Prints out byte values in all possible formats:
    As raw binary values
    As ASCII-encoded decimal, hex, octal, and binary values

  For more on ASCII, see http://www.asciitable.com and http://en.wikipedia.org/wiki/ASCII

  RECOMMENDED SETTINGS FOR THIS SKETCH
  ------------------------------------------------------------------------------

  Tools > Board          : ATtiny13
  Tools > BOD            : [Use any BOD level you like]
  Tools > Clock          : [Use any clock except 128 kHz]
  Tools > Timing         : Micros disabled

  SERIAL REMINDER
  ------------------------------------------------------------------------------
  The baud rate is IGNORED on the ATtiny13 due to using a simplified serial.
  The actual Baud Rate used is dependant on the processor speed.
  Note that you can specify a custom baud rate if the following ones does
  not fit your application.

  THESE CLOCKS USES 115200 BAUD:   THIS CLOCK USES 57600 BAUD:
  (External)  20 MHz               (Internal) 4.8 MHz
  (External)  16 MHz
  (External)  12 MHz
  (External)   8 MHz
  (Internal) 9.6 MHz

  THESE CLOCKS USES 19200 BAUD:    THIS CLOCK USES 9600 BAUD:
  (Internal) 1.2 MHz               (Internal) 600 KHz
  (External)   1 MHz

  If you get garbage output:
   1. Check baud rate as above
   2. Check if you have anything else connected to TX/RX like an LED
   3. Check OSCCAL (see OSCCAL tuner example)
*/

#include <EEPROM.h>

void setup()
{
  // Check if there exist any OSCCAL value in EEPROM addr. 0
  // If not, run the oscillator tuner sketch first
  uint8_t cal = EEPROM.read(0);
  if (cal < 0x7F)
    OSCCAL = cal;

  // Note that any baud rate specified is ignored on the ATtiny13. See header above.
  Serial.begin();

  // First visible ASCII character '!' is number 33
  // Last visible ASCII character '~' is number 126
  for (uint8_t asciiChar = 33; asciiChar < 127; asciiChar++)
  {
    // Prints value unaltered, i.e. the raw binary version of the
    // byte. The serial monitor interprets all bytes as
    // ASCII, so 33, the first number,  will show up as '!'
    Serial.print(F("Char: "));
    Serial.write(asciiChar);

    Serial.print(F(", dec: "));
    // Prints value as string as an ASCII-encoded decimal (base 10).
    // Decimal is the  default format for Serial.print() and Serial.println(),
    // so no modifier is needed:
    Serial.print(asciiChar);
    // But you can declare the modifier for decimal if you want to.
    // This also works if you uncomment it:

    // Serial.print(thisByte, DEC);

    Serial.print(F(", hex: "));
    // Prints value as string in hexadecimal (base 16):
    Serial.print(asciiChar, HEX);

    Serial.print(F(", bin: "));
    // prints value as string in binary (base 2)
    // also prints ending line break:
    Serial.print(asciiChar, BIN);
    Serial.write('\n');
  }
}

void loop()
{

}