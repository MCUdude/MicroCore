/*** MicroCore - wiring_shift.c ***
An Arduino core designed for ATtiny13
Based on the work done by "smeezekitty" 
Modified and maintained by MCUdude
https://github.com/MCUdude/MicroCore

This file contains the shift related 
functions shiftIn() and shiftOut().
*/

#include "wiring_private.h"

uint8_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder)
{  
  #if defined(SAFEMODE)
    if(clockPin > 5 || datapin > 5) // Return if pin number is too high
      return;
    if(clockPin < 2)
      turnOffPWM(clockPin); // If it's a PWM pin, make sure PWM is off
    if(dataPin < 2)
      turnOffPWM(dataPin);  // If it's a PWM pin, make sure PWM is off
  #endif  
 
  uint8_t value = 0;
  
  for(uint8_t i = 0; i < 8; ++i)
  {
    PORTB |= _BV(clockPin);  // Set clockPin high
    value |= (!!(PINB & _BV(dataPin)) << ((bitOrder == LSBFIRST) ? i : 7 - i)); // clock in to dataPin
    PORTB &= ~_BV(clockPin); // Set clockPin low
  }
  return value;
}


void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t value)
{
  
  const uint8_t datapinMask = _BV(dataPin);
  const uint8_t clkpinMask = _BV(clockPin);
  uint8_t i = 8;
  uint8_t portbits = (PORTB & ~(datapinMask | clkpinMask));
  
  #if defined(SAFEMODE)
    if(clockPin > 5 || dataPin > 5) // Return if pin number is too high
      return;
    if(clockPin < 2)
      turnOffPWM(clockPin); // If it's a PWM pin, make sure PWM is off
    if(dataPin < 2)
      turnOffPWM(dataPin);  // If it's a PWM pin, make sure PWM is off
  #endif    
   
  do
  {
    PORTB = portbits; // Clock and data pin low
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
  }
  while(--i);
}
