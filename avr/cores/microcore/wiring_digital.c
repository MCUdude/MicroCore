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
#include "core_settings.h"

void pinMode(uint8_t pin, uint8_t mode)
{
  // SAFEMODE prevents you from inserting a pin number out of range
  #ifdef SAFEMODE
    if(pin > 5)
      return;
  #endif
  
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
  // SAFEMODE prevents you from inserting a pin number out of range, and disables PWM if turned on
  #if defined(SAFEMODE)
    if(pin > 5)
      return;
    if(pin < 2)
      turnOffPWM(pin); // If it's a PWM pin, make sure PWM is off
  #endif  
    
  if(val)
    PORTB |= _BV(pin);  // Set pin high
  else
    PORTB &= ~_BV(pin); // Set pin low
}


uint8_t digitalRead(uint8_t pin)
{
  // SAFEMODE prevents you from inserting a pin number out of range, and disables PWM if turned on
  #ifdef SAFEMODE
    if(pin > 5)
      return 0;
    if(pin < 2)
      turnOffPWM(pin); // If it's PWM pin, makes sure the PWM is off
  #endif
  
  return !!(PINB & _BV(pin));
}
