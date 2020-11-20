/*** MicroCore - wiring_digital.c ***
An Arduino core designed for ATtiny13
Based on the work done by "smeezekitty" 
Modified and maintained by MCUdude
https://github.com/MCUdude/MicroCore

This file contains digital related 
functions such as pinMode(),
digitalWrite() and digitalRead().
*/

#include "wiring_private.h"

void pinMode(uint8_t pin, uint8_t mode)
{
 check_valid_digital_pin(pin);
  
  if(mode == OUTPUT) // Pin as output
    DDRB |= _BV(pin);
      
  else // Pin as input or input pullup
  {
    DDRB &= ~_BV(pin); // Set pin as input
    if(mode == INPUT_PULLUP)
      PORTB |= _BV(pin); // Enable pullup resistors
  }
}


void digitalWrite(uint8_t pin, uint8_t val)
{
  check_valid_digital_pin(pin);
    
  if(val)
    PORTB |= _BV(pin);  // Set pin high
  else
    PORTB &= ~_BV(pin); // Set pin low
}


uint8_t digitalRead(uint8_t pin)
{
  check_valid_digital_pin(pin);
  
  return !!(PINB & _BV(pin));
}
