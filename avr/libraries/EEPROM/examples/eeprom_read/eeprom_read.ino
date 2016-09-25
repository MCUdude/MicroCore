/*
 * EEPROM Read
 *
 * Reads the value of each byte of the EEPROM and prints it
 * to the computer.
 * This example code is in the public domain.
 */

#include <EEPROM.h>

// start reading from the first byte (address 0) of the EEPROM
int address = 0;
byte value;

void setup() {
  
  /* If you want to implement serial output on the ATtiny13,
	you should check out Ralph Doncaster's BasicSerial3 library!
	http://nerdralph.blogspot.no/2014/01/avr-half-duplex-software-uart.html
	*/
  
  Serial.begin(9600);

}

void loop() {
  // read a byte from the current address of the EEPROM
  value = EEPROM.read(address);

  //Serial.print(address);
  //Serial.print("\t");
  //Serial.print(value, DEC);
  //Serial.println();

  /***
    Advance to the next address, when at the end restart at the beginning.

    Larger AVR processors have larger EEPROM sizes, E.g:
    - Arduno Duemilanove: 512b EEPROM storage.
    - Arduino Uno:        1kb EEPROM storage.
    - Arduino Mega:       4kb EEPROM storage.

    Rather than hard-coding the length, you should use the pre-provided length function.
    This will make your code portable to all AVR processors.
  ***/
  address = address + 1;
  if (address == EEPROM.length()) {
    address = 0;
  }

  /***
    As the EEPROM sizes are powers of two, wrapping (preventing overflow) of an
    EEPROM address is also doable by a bitwise and of the length - 1.

    ++address &= EEPROM.length() - 1;
  ***/

  delay(500);
}
