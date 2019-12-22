/*
  Print a table of ASCII characters to the Serial Monitor

  By J.Sleeman (sparks@gogo.co.nz)
  MicroCore support by MCUdude
  ------------------------------------------------------------------------------

  [ See diagram: https://github.com/MCUdude/MicroCore#minimal-setup ]

  Prints out byte values in all possible formats:
  * As raw binary values
  * As ASCII-encoded decimal, hex, octal, and binary values

  For more on ASCII, see http://www.asciitable.com and http://en.wikipedia.org/wiki/ASCII

  RECOMMENDED SETTINGS FOR THIS SKETCH
  ------------------------------------------------------------------------------

  Tools > Board          : ATtiny13
  Tools > BOD            : [Use any BOD level you like]
  Tools > Compiler LTO   : LTO enabled
  Tools > Clock          : [Use any clock except 128 kHz]
  Tools > Timing         : Millis enabled, Micros disabled

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


// First visible ASCII character '!' is number 33:
uint8_t thisByte = 33;
// You can also write ASCII characters in single quotes.
// For example; '!' is the same as 33, so you could also use this:
// uint8_t thisByte = '!';

void setup()
{
  // Note that any baud rate specified is ignored on the ATtiny13. See header above.
  Serial.begin();

  // Prints title with ending line break
  Serial.print(F("ASCII Table ~ Character Map\n"));

  // Wait for serial monitor to open and you to react
  delay(2000);
}


void loop()
{
  // Prints value unaltered, i.e. the raw binary version of the
  // byte. The serial monitor interprets all bytes as
  // ASCII, so 33, the first number,  will show up as '!'
  Serial.print(F("Char: "));
  Serial.write(thisByte);

  Serial.print(F(", dec: "));
  // Prints value as string as an ASCII-encoded decimal (base 10).
  // Decimal is the  default format for Serial.print() and Serial.println(),
  // so no modifier is needed:
  Serial.print(thisByte);
  // But you can declare the modifier for decimal if you want to.
  // This also works if you uncomment it:

  // Serial.print(thisByte, DEC);

  Serial.print(F(", hex: "));
  // Prints value as string in hexadecimal (base 16):
  Serial.print(thisByte, HEX);

  Serial.print(F(", bin: "));
  // prints value as string in binary (base 2)
  // also prints ending line break:
  Serial.print(thisByte, BIN);
  Serial.write('\n');

  // if printed last visible character '~' or 126, stop:
  if(thisByte == 126)  // you could also use if (thisByte == '~')
  {
    // This loop loops forever and does nothing
    while(true);
  }
  
  // Go on to the next character
  thisByte++;
}