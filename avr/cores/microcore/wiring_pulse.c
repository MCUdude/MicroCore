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
  uint32_t st,to;
  to = micros();
  while(digitalRead(pin) == stat){if((micros() - to) > timeout){return 0;}}
  while(digitalRead(pin) != stat){if((micros() - to) > timeout){return 0;}}
  st = micros();
  while(digitalRead(pin) == stat){if((micros() - to) > timeout){return 0;}}
  return micros()-st;
}
  
