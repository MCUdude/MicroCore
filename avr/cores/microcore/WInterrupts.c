/*** MicroCore - WInterrupts.c ***
An Arduino core designed for ATtiny13
Based on the work done by "smeezekitty" 
Modified and maintained by MCUdude
*/

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdio.h>

#include "wiring_private.h"
#include "core_settings.h"

static volatile voidFuncPtr intFunc[NUMBER_EXTERNAL_INTERRUPTS];

void attachInterrupt(uint8_t interruptNum, void (*userFunc)(void), int mode) 
{
	#ifdef SAFEMODE
  if(interruptNum < NUMBER_EXTERNAL_INTERRUPTS) {
  #endif
    uint8_t SaveSREG = SREG; // Save interrupt flag

    cli(); // Disable interrupts
    
    intFunc[interruptNum] = userFunc; // access the shared data
    
    SREG = SaveSREG; // Restore the interrupt flag
    
    // Configure the interrupt mode (trigger on low input, any change, rising
    // edge, or falling edge).  The mode constants were chosen to correspond
    // to the configuration bits in the hardware register, so we simply shift
    // the mode into place.
      
    // Enable INT0 on pin PB1
    MCUCR = (MCUCR & ~((1 << ISC00) | (1 << ISC01))) | (mode << ISC00);
    GIMSK |= (1 << INT0);
  #ifdef SAFEMODE  
  }
  #endif
}


void detachInterrupt(uint8_t interruptNum) 
{
	#ifdef SAFEMODE
  if (interruptNum < NUMBER_EXTERNAL_INTERRUPTS) 
  {
  #endif
    // Disable INT0 on pin PB1
    GIMSK &= ~(1 << INT0);
    intFunc[interruptNum] = 0;
  #ifdef SAFEMODE  
  }
  #endif
}


ISR(INT0_vect)
{
  if(intFunc[EXTERNAL_INTERRUPT_0])
    intFunc[EXTERNAL_INTERRUPT_0]();
}
