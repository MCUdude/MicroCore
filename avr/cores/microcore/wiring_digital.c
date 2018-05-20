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
    #if defined(__AVR_ATtiny13__)
      if(pin > 5)
        return;
    #else
      if(pin > 3)
        return;
    #endif
  #endif
  
  if(mode == OUTPUT) // Pin as output
    DDRB |= _BV(pin);
      
  else // Pin as input or input pullup
  {
    DDRB &= ~_BV(pin); // Set pin as input
    if(mode == INPUT_PULLUP)
      #if defined(__AVR_ATtiny13__)
        PORTB |= _BV(pin); // Enable pullup resistors
      #else
        PUEB |= _BV(pin); // Enable pullup resistors
      #endif
  }
}


void digitalWrite(uint8_t pin, uint8_t val)
{
  // SAFEMODE prevents you from inserting a pin number out of range, and disables PWM if turned on
  #if defined(SAFEMODE)
    #if defined(__AVR_ATtiny13__)
      if(pin > 5)
        return;
    #else
      if(pin > 3)
        return; 
    #endif
    if(pin < 2)
      turnOffPWM(pin); // If it's a PWM pin, make sure PWM is off
    if(pin > 5)
      return;
    #if defined(SETUP_PWM)
      if(pin < 2)
        turnOffPWM(pin); // If it's a PWM pin, make sure PWM is off
    #endif
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
    #if defined(__AVR_ATtiny13__)
      if(pin > 5)
        return;
    #else
      if(pin > 3)
        return; 
    #endif
    if(pin < 2)
      turnOffPWM(pin); // If it's PWM pin, makes sure the PWM is off
    if(pin > 5)
      return 0;
    #if defined(SETUP_PWM)
      if(pin < 2)
        turnOffPWM(pin); // If it's a PWM pin, make sure PWM is off
    #endif
  #endif
  
  return !!(PINB & _BV(pin));
}
