/*
  Read string from Serial, Print string to Serial

  By J.Sleeman (sparks@gogo.co.nz)
  MicroCore support by MCUdude
  ------------------------------------------------------------------------------

  [ See diagram: https://github.com/MCUdude/MicroCore#minimal-setup ]

  Asks your name, says hello. Not very exciting, but anyway.

  There is an important note here with regard to reading strings over
  Serial on the ATtiny13.  Because of a lack of hardware serial support
  interrupts are disabled for the ENTIRE READ of the string. This means that
  millis() will "lose time" for the period you are reading the string.
  Just bear that in mind. Remember to turn on newline in the serial monitor!

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
  if(cal < 0x7F)
    OSCCAL = cal;

  // Note that any baud rate specified is ignored on the ATtiny13. See header above.
  Serial.begin();
}

void loop()
{
  char buf[15];
  Serial.println(F("What is your name traveler?"));
  Serial.read_str(buf, sizeof(buf), '\n'); // Exit on newline
  Serial.print(F("Nice to meet you "));
  Serial.println(buf);
}
