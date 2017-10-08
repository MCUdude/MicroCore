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


// COMPILER_LTO is automatically defined in the platforms.txt when an LTO option is selected in the GCC flags menu
#if defined(COMPILER_LTO) // Choose the function below if LTO is enabled

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
    uint32_t maxloops = microsecondsToClockCycles(timeout) >> 4; // Same as dividing by 16
    uint32_t width = 0; // Keep initialization out of time critical area

    // Wait for any previous pulse to end
    while(!!(PINB & _BV(pin)) == state)
    {
      if(numloops++ == maxloops) {return 0;}
      asm("nop \n");  
      asm("nop \n");
    }
    // Wait for the pulse to start
    while(!!(PINB & _BV(pin)) != state)
    {
      if(numloops++ == maxloops) {return 0;}
      asm("nop \n");  
      asm("nop \n");
      asm("nop \n");
    }
    // Wait for the pulse to stop This loop is 16 instructions long
    while(!!(PINB & _BV(pin)) == state)
    {
      if(numloops++ == maxloops) {return 0;}           
      width++;
      asm("nop \n");  
      asm("nop \n");  
      asm("nop \n"); 
    }

    // Convert the reading to microseconds. 
    return clockCyclesToMicroseconds(width << 4); // Multiply by 16
}


#else // Choose the function below if LTO is disabled
  
  // This pulseIn function is based on micros, and _will_ be inaccurate
  // if F_CPU doesn't divide evenly by 64
  uint32_t pulseIn(uint8_t pin, uint8_t stat, uint32_t timeout)
  { 
  
   #if defined(SAFEMODE)
     if(pin > 5) // Return if pin number is too high
       return;
     if(pin < 2)
       turnOffPWM(c); // If it's a PWM pin, make sure PWM is off
    #endif
  
    uint32_t st,to;
    to = micros();
    while(!!(PINB & _BV(pin)) == stat){if((micros() - to) > timeout){return 0;}}
    while(!!(PINB & _BV(pin)) != stat){if((micros() - to) > timeout){return 0;}}
    st = micros();
    while(!!(PINB & _BV(pin)) == stat){if((micros() - to) > timeout){return 0;}}
    return micros()-st;
  }
#endif





