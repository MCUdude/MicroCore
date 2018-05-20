/*** MicroCore - WInterrupts.c ***
An Arduino core designed for ATtiny13
Based on the work done by "smeezekitty" 
Modified and maintained by MCUdude
https://github.com/MCUdude/MicroCore

This fine contains interrupt functions
such as attachInterrupt() and
detachInterrupt().
*/

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdio.h>

#include "wiring_private.h"
#include "core_settings.h"

#define EXTERNAL_INTERRUPT_0 0

static volatile voidFuncPtr intFunc;

void attachInterrupt(uint8_t interruptNum, void (*userFunc)(void), uint8_t mode) 
{
  // The ATtiny13 only has one interrupt
  (void)interruptNum;
  
  #if !defined(SAFEMODE)
    uint8_t SaveSREG = SREG; // Save interrupt flag
  #endif
  
  cli(); // Disable interrupts
    
  intFunc = userFunc; // Access the shared data
  
  #if !defined(SAFEMODE)
    SREG = SaveSREG; // Restore the interrupt flag
  #else
    sei(); // Enable global interrupts  
  #endif  
  
    
  // Configure the interrupt mode (trigger on low input, any change, rising
  // edge, or falling edge).  The mode constants were chosen to correspond
  // to the configuration bits in the hardware register, so we simply shift
  // the mode into place.
      
  // Enable INT0 on pin PB1
  #if defined(__AVR_ATtiny10__)||defined(__AVR_ATtiny9__)
    EICRA = (EICRA & ~(_BV(ISC00) | (ISC01))) | (mode << ISC00);
    EIMSK |= _BV(INT0);	
  #else
    MCUCR = (MCUCR & ~(_BV(ISC00) | (ISC01))) | (mode << ISC00);
    GIMSK |= _BV(INT0);
  #endif
  
}


void detachInterrupt(uint8_t interruptNum) 
{
  // The ATtiny13 only has one interrupt
  (void)interruptNum;
  
  // Disable INT0 on pin PB1
  #if defined(__AVR_ATtiny10__)||defined(__AVR_ATtiny9__)
    EIMSK &= ~_BV(INT0);
  #else
    GIMSK &= ~_BV(INT0);
  #endif
  intFunc = 0; 
}


// AttachInterrupt ISR
ISR(INT0_vect)
{
  intFunc();
}
