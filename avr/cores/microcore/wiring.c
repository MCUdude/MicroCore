/*** MicroCore - wiring.c ***
An Arduino core designed for ATtiny13
Based on the work done by "smeezekitty"
Modified and maintained by MCUdude
https://github.com/MCUdude/MicroCore

This file contains timing related
functions such as millis(), micros(),
delay() and delayMicroseconds(), but
also the init() function that set up
timers.
*/

#include "wiring_private.h"
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/wdt.h>
#include "core_settings.h"

/***** MILLIS() *****/
// The millis counter is based on the watchdog timer, and takes very little processing time and power.
// If 16 ms accuracy is enough, I strongly recommend you to use millis() instead of micros().
// The WDT uses it's own clock, so this function is valid for all F_CPUs.

// C wrapper function for millis asm code
// reduces compiler register pressure vs avr-gcc calling convention
// http://nerdralph.blogspot.ca/2015/12/lightweight-avr-assembler-functions.html


/**
 * @brief Returns the number of milliseconds passed since the microcontroller
 *        began running the current program.
 *
 * @return uint32_t Number of milliseconds passed since the program started
 */
uint32_t millis()
{
  uint32_t m;
  asm volatile ("rcall _millis" : "=w" (m) :: "r30");
  return m;
}


/**
 * @brief Pauses the program for the amount of time (in milliseconds)
 *        specified as parameter.
 *
 * @param ms The number of milliseconds to pause
 */
void delay(uint16_t ms)
{
  while(ms--)
    _delay_ms(1);
}


/**
 * @brief Initializing function that runs before setup().
 *        This is where timer0 is set up to count micros() if enabled.
 */
void init()
{
  // WARNING! Enabling micros() will affect timing functions!
  #ifdef ENABLE_MICROS
    // Set a suited prescaler based on F_CPU
    #if F_CPU >= 4800000L
      TCCR0B = _BV(CS00) | _BV(CS01); // F_CPU/64
    #else
      TCCR0B = _BV(CS01);             // F_CPU/8
    #endif
    // Enable overflow interrupt on Timer0
    TIMSK0 = _BV(TOIE0);
    // Set timer0 couter to zero
    TCNT0 = 0;
  #endif

  // Turn on global interrupts
  sei();
}
