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
    else if(dataPin < 2)
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


void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val)
{
  
  #if defined(SAFEMODE)
    if(clockPin > 5 || dataPin > 5) // Return if pin number is too high
      return;
    if(clockPin < 2)
      turnOffPWM(clockPin); // If it's a PWM pin, make sure PWM is off
    else if(dataPin < 2)
      turnOffPWM(dataPin);  // If it's a PWM pin, make sure PWM is off
  #endif    
  
  for(uint8_t i = 0; i < 8; i++)  
  {
    if(bitOrder == LSBFIRST)
      (!!(val & _BV(i))) ? (PORTB |= _BV(dataPin)) : (PORTB &= ~_BV(dataPin));
    else
      (!!(val & _BV(7 - i))) ? (PORTB |= _BV(dataPin)) : (PORTB &= ~_BV(dataPin));

    PORTB |= _BV(clockPin);  // Set pin clockPin high
    asm("nop");              // Wait one instruction
    PORTB &= ~_BV(clockPin); // Set clockPin low
  }
}
