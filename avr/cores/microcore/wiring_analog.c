/*** MicroCore - wiring_analog.c ***
An Arduino core designed for ATtiny13
Based on the work done by "smeezekitty" 
Modified and maintained by MCUdude
https://github.com/MCUdude/MicroCore

This file contains analog related
functions such as analogRead()
and analogReference().
*/

#include "wiring_private.h"


void analogReference(uint8_t mode)
{
   if(mode == INTERNAL)
      ADMUX |= _BV(REFS0);
   else
      ADMUX &= ~_BV(REFS0);
}


int16_t analogRead(uint8_t pin)
{
  uint8_t l,h;
  ADMUX = (ADMUX & _BV(REFS0)) | (pin & 0x03); // Setup ADC, preserve REFS0
  ADCSRA |= _BV(ADSC);  
  while(ADCSRA & _BV(ADSC)); // Wait for conversion
  l = ADCL;  // Read and return 10 bit result
  h = ADCH;
  return (h << 8)|l; 
}


    
