/*** MicroCore - wiring_shift.c ***
An Arduino core designed for ATtiny13
Based on the work done by "smeezekitty"
Modified and maintained by MCUdude
https://github.com/MCUdude/MicroCore

This file contains the shift related
functions shiftIn() and shiftOut().
*/

#include "wiring_private.h"


/**
 * @brief Shifts in a byte of data one bit at a time. Starts from either the
 *        most (i.e. the leftmost) or least (rightmost) significant bit. For
 *        each bit, the clock pin is pulled high, the next bit is read from the
 *        data line, and then the clock pin is taken low.
 *
 * @param dataPin Digital input pin to receive serial data
 * @param clockPin The pin to toggle to signal a read from dataPin
 * @param bitOrder Which order to shift in the bits
 *        MSBFIRST or LSBFIRST are valid options
 * @return uint8_t The value read
 */
uint8_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder)
{
  uint8_t value = 0;
  uint8_t i = 8;
  do
  {
    PORTB |= _BV(clockPin);
    if(bitOrder == MSBFIRST)
      value <<= 1;
    else
      value >>= 1;
    if(PINB & _BV(dataPin))
    {
      if(bitOrder == MSBFIRST)
        value |= 0x01;
      else
        value |= 0x80;
    }
    PORTB &= ~_BV(clockPin);
  }
  while(--i);

  return value;
}


/**
 * @brief Shifts out a byte of data one bit at a time. Starts from either the
 *        most (i.e. the leftmost) or least (rightmost) significant bit. Each
 *        bit is written in turn to a data pin, after which a clock pin is
 *        pulsed (taken high, then low) to indicate that the bit is available.
 *
 * @param dataPin The pin on which to output each bit
 * @param clockPin The pin to toggle once the dataPin has been set to the correct value
 * @param bitOrder Which order to shift in the bits
 *        MSBFIRST or LSBFIRST are valid options
 * @param value The data to shift out
 */
void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t value)
{
  const uint8_t datapinMask = _BV(dataPin);
  const uint8_t clkpinMask = _BV(clockPin);
  uint8_t i = 8;
  uint8_t portbits = (PORTB &= ~(datapinMask | clkpinMask));

  do
  {
    if(bitOrder == MSBFIRST)
    {
      if(value & 0x80)
        PINB = datapinMask;
    }
    else
    {
      if(value & 0x01)
        PINB = datapinMask;
    }
    PINB = clkpinMask; // Toggle clock pin
    if(bitOrder == MSBFIRST)
      value <<= 1;
    else
      value >>= 1;
    PORTB = portbits; // Clock and data pin low
  }
  while(--i);
}
