/*** MicroCore - wiring_pulse.c ***
An Arduino core designed for ATtiny13
Based on the work done by "smeezekitty"
Modified and maintained by MCUdude
https://github.com/MCUdude/MicroCore

This file contains the pulseIn() function,
which measures the length (in microseconds)
of a pulse on the attached pin. puseIn() can
measure the HIGH duration or the LOW duration.
Works on pulses from 2-3 microseconds to 3 minutes
in length, but must be called at least a few
dozen microseconds before the start of the pulse.
*/

#include "wiring_private.h"


// This pulseIn function is a complete rewrite of the original one,
// and does not depend on micros() to work.
// However it will only work properly with LTO enabled.
uint32_t pulseIn(uint8_t pin, uint8_t state, uint32_t timeout)
{

  #if defined(SAFEMODE)
    if(pin > 5) // Return if pin number is too high
      return;
    if(pin < 2)
      turnOffPWM(c); // If it's a PWM pin, make sure PWM is off
  #endif

  // Convert the timeout from microseconds to a number of times through
  // the initial loop; it takes 16 clock cycles per iteration.
  uint32_t numloops = 0;
  uint32_t maxloops = microsecondsToClockCycles(timeout); // Same as dividing by 16
  __uint24 width = 0; // Keep initialization out of time critical area

  // Wait for any previous pulse to end
  while(!!(PINB & _BV(pin)) == state)
  {
    if(numloops++ >= maxloops) {return 0;}
    asm("nop");
    asm("nop");
  }
  // Wait for the pulse to start
  while(!!(PINB & _BV(pin)) != state)
    if(++numloops >= maxloops) {return 0;}

  // Wait for the pulse to stop This loop is 16 instructions long
  while(!!(PINB & _BV(pin)) == state)
  {
    if(numloops++ >= maxloops) {return 0;}
    ++width;
  }

  // Convert the reading to microseconds.
  return (width); // Multiply by 16
}
