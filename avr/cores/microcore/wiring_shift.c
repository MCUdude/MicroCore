/*** MicroCore - wiring_shift.c ***
An Arduino core designed for ATtiny13
Based on the work done by "smeezekitty" 
Modified and maintained by MCUdude
https://github.com/MCUdude/MicroCore

This file contains the shift related 
functions shiftIn() and shiftOut().
*/

#include "wiring_private.h"

uint8_t shiftIn(uint8_t d, uint8_t c, uint8_t bitor) 
{  
  #if defined(SAFEMODE)
    if(c > 5 || d > 5) // Return if pin number is too high
      return;
    if(c < 2)
      turnOffPWM(c); // If it's a PWM pin, make sure PWM is off
    else if(d < 2)
      turnOffPWM(d); // If it's a PWM pin, make sure PWM is off
  #endif  
 
  uint8_t value = 0;
  
  for(uint8_t i = 0; i < 8; ++i)
  {
    PORTB |= _BV(c);  // Set pin c high
    value |= (!!(PINB & _BV(d)) << ((bitor == LSBFIRST) ? i : 7 - i)); // clock in the data to pin d
    PORTB &= ~_BV(c); // Set pin c low
  }
  return value;
}


void shiftOut(uint8_t d, uint8_t c, uint8_t bitor, uint8_t val)
{
  
  #if defined(SAFEMODE)
    if(c > 5 || d > 5) // Return if pin number is too high
      return;
    if(c < 2)
      turnOffPWM(c); // If it's a PWM pin, make sure PWM is off
    else if(d < 2)
      turnOffPWM(d); // If it's a PWM pin, make sure PWM is off
  #endif    
  
  for(uint8_t i = 0; i < 8; i++)  
  {
    if(bitor == LSBFIRST)
      (!!(val & _BV(i))) ? (PORTB |= _BV(d)) : (PORTB &= ~_BV(d));
    else
      (!!(val & _BV(7 - i))) ? (PORTB |= _BV(d)) : (PORTB &= ~_BV(d));

    PORTB |= _BV(c);  // Set pin c high
    asm("nop");       // Wait one instruction
    PORTB &= ~_BV(c); // Set pin c low
  }
}
