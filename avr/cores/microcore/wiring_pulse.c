/*** MicroCore - wiring_pulse.c ***
An Arduino core designed for ATtiny13
Based on the work done by "smeezekitty" 
Modified and maintained by MCUdude
https://github.com/MCUdude/MicroCore

This file contains the pulseIn() function,
which depend on micros().
*/

#include "wiring_private.h"

uint32_t pulseIn(uint8_t pin, uint8_t stat, uint32_t timeout)
{ 
  #if defined(SAFEMODE)
    if(c > 5 || d > 5) // Return if pin number is too high
      return;
    if(c < 2)
      turnOffPWM(c); // If it's a PWM pin, make sure PWM is off
    else if(d < 2)
      turnOffPWM(d); // If it's a PWM pin, make sure PWM is off
  #endif   
  
  uint32_t st,to;
  to = micros();
  while(!!(PINB & _BV(pin)) == stat){if((micros() - to) > timeout){return 0;}}
  while(!!(PINB & _BV(pin)) != stat){if((micros() - to) > timeout){return 0;}}
  st = micros();
  while(!!(PINB & _BV(pin)) == stat){if((micros() - to) > timeout){return 0;}}
  return micros()-st;
}
  
