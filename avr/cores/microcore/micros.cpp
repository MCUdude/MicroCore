#include <avr/interrupt.h>
#include "wiring_private.h"

#define MICROS_ENABLED

// The trick here is to let avr-gcc optimize away the micros() function
// and the initialization function init_micros(). When micros() is present in the
// user application, init_micros() is actually ran _before_ main()!
void init_micros(void) __attribute__ ((naked)) __attribute__ ((used)) __attribute__ ((section (".init6")));

void init_micros(void)
{
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
}

// Enabling micros() will cause the processor to interrupt more often (every 2048th clock cycle if
// F_CPU < 4.8 MHz, every 16384th clock cycle if F_CPU >= 4.8 MHz. This will add some overhead when F_CPU is
// less than 4.8 MHz. It's disabled by default because it occupies precious flash space and loads the CPU with
// additional interrupts and calculations. Also note that micros() aren't very precise for frequencies that 64
// doesn't divide evenly by, such as 9.6 and 4.8 MHz.
// timer0 count variable
volatile uint32_t timer0_overflow = 0;
// This will cause an interrupt every 256*64 clock cycle

ISR(TIM0_OVF_vect)
{
  timer0_overflow++; // Increment counter by one
}

/**
 * @brief Returns the number of microseconds since the microcontroller
 *        began running the current program.
 *
 * @return uint32_t Number of microseconds passed since the program started
 */
uint32_t micros()
{
  uint32_t x;
  uint8_t t;

  uint8_t oldSREG = SREG; // Preserve old SREG value
  t = TCNT0;              // Store timer0 counter value
  cli();                  // Disable global interrupts
  x = timer0_overflow;    // Store timer0 overflow count
  SREG = oldSREG;         // Restore SREG

  #if F_CPU == 20000000L
    // Each timer tick is 1/(16MHz/64) = 3.2us long. We multiply the timer0_overflow variable
    // by 256 (bitshift 8 times) and we add the current timer count TCNT0. Since each tick is 3.2us long,
    // we multiply by 3 at the end
    return ((x << 8) + t) * 3;
  #elif F_CPU == 16000000L
    // Each timer tick is 1/(16MHz/64) = 4us long. We multiply the timer0_overflow variable
    // by 256 (bitshift 8 times) and we add the current timer count TCNT0. Since each tick is 4us long,
    // we multiply by 4 at the end
    return ((x << 8) + t) * 4;
  #elif F_CPU == 12000000L
    // Each timer tick is 1/(12MHz/64) = 5.333us long. We multiply the timer0_overflow variable
    // by 256 (bitshift 8 times) and we add the current timer count TCNT0. Since each tick is 5.333us long,
    // we multiply by 5 at the end
    return ((x << 8) + t) * 5;
  #elif F_CPU == 9600000L
    // Each timer tick is 1/(9.6MHz/64) = 6.666us long. We multiply the timer0_overflow variable
    // by 256 (bitshift 8 times) and we add the current timer count TCNT0. Since each tick is 6.666us long,
    // we multiply by 7 at the end
    return ((x << 8) + t) * 7;
  #elif F_CPU == 8000000L
    // Each timer tick is 1/(8MHz/64) = 8us long. We multiply the timer0_overflow variable
    // by 256 (bitshift 8 times) and we add the current timer count TCNT0. Since each tick is 8us long,
    // we multiply by 8 at the end
    return ((x << 8) + t) * 8;
  #elif F_CPU == 4800000L
    // Each timer tick is 1/(4.8MHz/64) = 13.333us long. We multiply the timer0_overflow variable
    // by 256 (bitshift 8 times) and we add the current timer count TCNT0. Since each tick is 13.333us long,
    // we multiply by 13 at the end
    return ((x << 8) + t) * 13;
  #elif F_CPU == 1200000L
    // Each timer tick is 1/(1.2MHz/8) = 6.666us long. We multiply the timer0_overflow variable
    // by 256 (bitshift 8 times) and we add the current timer count TCNT0. Since each tick is 6.666us long,
    // we multiply by 7 at the end
    return ((x << 8) + t) * 7;
  #elif F_CPU == 1000000L
    // Each timer tick is 1/(1MHz/8) = 8us long. We multiply the timer0_overflow variable
    // by 256 (bitshift 8 times) and we add the current timer count TCNT0. Since each tick is 8us long,
    // we multiply by 8 at the end
    return ((x << 8) + t) * 8;
  #elif F_CPU == 600000L
    // Each timer tick is 1/(600kHz/8) = 13.333us long. We multiply the timer0_overflow variable
    // by 256 (bitshift 8 times) and we add the current timer count TCNT0. Since each tick is 13.333us long,
    // we multiply by 13 at the end
    return ((x << 8) + t) * 13;
  #elif F_CPU == 128000L
    // Each timer tick is 1/(128kHz/8) = 62.5us long. We multiply the timer0_overflow variable
    // by 256 (bitshift 8 times) and we add the current timer count TCNT0. Since each tick is 62.5us long,
    // we multiply by 62 at the end
    return ((x << 8) + t) * 62;
 #endif
}
