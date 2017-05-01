/*** MicroCore - wiring.c ***
An Arduino core designed for ATtiny13
Based on the work done by "smeezekitty" 
Modified and maintained by MCUdude
https://github.com/MCUdude/MicroCore

This file contains timing related
functions such as millis(), micros(),
delay() and delayMicroseconds(), but
also the init() function that set up
timers and analog related stuff.
*/

#include "wiring_private.h"
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/wdt.h>
#include "core_settings.h"

/***** MILLIS() *****/
// The millis counter is based on the watchdog timer, and takes very little processing time and power.
// If 16 ms accuracy is enough, I strongly recommend you to use millis() instead of micros().
#ifdef ENABLE_MILLIS
volatile uint32_t wdt_interrupt_counter = 0;

// This ISR will execute every 16 ms, and increase 
ISR(WDT_vect)
{
  wdt_interrupt_counter++;
}

// Since the WDT counter counts every 16th ms, we'll need to
// multiply by 16 to get the correct millis value.
// The WDT uses it's own clock, so this function is valid
// for all F_CPUs.
uint32_t millis()
{  
  return wdt_interrupt_counter * 16;
}
#endif // ENABLE_MILLIS


/***** MICROS() *****/
// To achieve accurate micros() readings, we'll have to enable the overflow interrupt
// vector on timer 0. This means there will be an interrupt every 256 clock cycle.
// Interrupts as rapidly as this tends to affect the overall time keeping.
// E.g if micros() is enabled, the delay(1) function will actually last 1.3 ms instead.
#ifdef ENABLE_MICROS
volatile uint32_t timer0_overflow = 0;

// This will cause an interrupt every 256 clock cycle
ISR(TIM0_OVF_vect)
{
  timer0_overflow++; // Increment counter by one
}


uint32_t micros()
{
  uint32_t x;
  cli();
  #if F_CPU == 16000
    x = timer0_overflow * 16000;
  #elif F_CPU < 150000 && F_CPU > 80000
    x = timer0_overflow * 2000;
  #elif F_CPU == 600000
    x = timer0_overflow * 427;
  #elif F_CPU == 1000000
    x = timer0_overflow * 256;
  #elif F_CPU == 1200000
    x = timer0_overflow * 213;
  #elif F_CPU == 4000000
    x = timer0_overflow * 64;
  #elif F_CPU == 4800000
    x = timer0_overflow * 53;
  #elif F_CPU == 8000000
    x = timer0_overflow * 32;
  #elif F_CPU == 9600000
    x = timer0_overflow * 27;
  #elif F_CPU == 10000000
    x = timer0_overflow * 26;
  #elif F_CPU == 12000000
    x = timer0_overflow * 21;
  #elif F_CPU == 16000000
    x = timer0_overflow * 16;
  #else 
  #error 
    #warning This CPU frequency is not defined (choose 128 kHz or more)
    sei();
    return 0;
  #endif
  sei();
  return x;
}
#endif // ENABLE_MICROS


// Wrapper to deal with _delay_ms(), which is an inline function
void delay(uint16_t ms)
{
  do
    _delay_ms(1);
  while(--ms);
    
}


// This init() function will be executed before the setup() function does
// Edit the core_settings.h file to choose what's going to be initialized 
// and what's not.
void init()
{
  #ifdef SETUP_PWM  
    // Set Timer0 prescaler
    #if defined(PRESCALER_NONE)        // PWM frequency = (F_CPU/256) / 1
      TCCR0B |= _BV(CS00);
    #elif  defined(PRESCALER_8)        // PWM frequency = (F_CPU/256) / 8
      TCCR0B |= _BV(CS01);
    #elif  defined(PRESCALER_64)      // PWM frequency = (F_CPU/256) / 64
      TCCR0B |= _BV(CS00) | _BV(CS01);
    #elif  defined(PRESCALER_256)      // PWM frequency = (F_CPU/256) / 256
      TCCR0B |= _BV(CS02);
    #elif  defined(PRESCALER_1024)    // PWM frequency = (F_CPU/256) / 1024
      TCCR0B |= _BV(CS00) | _BV(CS02);
    #endif
    
    // Set waveform generation mode
    #if defined(PWM_FAST)
      TCCR0A |= _BV(WGM00) | _BV(WGM01);
    #elif defined(PWM_NORMAL)
      TCCR0A &= ~_BV(WGM00) | ~_BV(WGM01);
    #elif defined(PWM_PHASE_CORRECT)
      TCCR0A |= _BV(WGM00);
    #elif defined(PWM_CTC)
      TCCR0A |= _BV(WGM01);
    #endif  
  #endif  
  
  // Enable WDT interrupt and enable global interrupts  
  #ifdef ENABLE_MILLIS
    // Disable global interrupts      
    cli();
    // Reset watchdog
    wdt_reset();
    // Set up WDT interrupt with 16 ms prescaler
    WDTCR = _BV(WDTIE);
    // Enable global interrupts
    sei();
  #endif
  
  // WARNING! Enabling micros() will affect timing functions heavily!
  #ifdef ENABLE_MICROS
    // Enable overflow interrupt on Timer0
    TIMSK0 |= _BV(TOIE0);
    // Set timer0 couter to zero
    TCNT0 = 0; 
    // Turn on global interrupts
    sei();
  #endif
  
  // Enable the ADC and set the prescaler according to the clock frequency
  #ifdef SETUP_ADC
    ADMUX = 0;
    // Less or equal to 200 kHz
    #if F_CPU <= 200000 
      // Enable the ADC, keep the prescaler of 2 --> F_CPU / 2
      ADCSRA |= _BV(ADEN);
      
    // Between 200 kHz and 1.2 MHz  
    #elif F_CPU <= 1200000 
      // Enable the ADC, set the prescaler to 4 --> F_CPU / 4
      ADCSRA |= _BV(ADEN) | _BV(ADPS1);
      
    // Between 1.2 MHz and 6.4 MHz
    #elif F_CPU <= 6400000
      // Enable the ADC, set the prescaler to 16 --> F_CPU / 16
      ADCSRA |= _BV(ADEN) | _BV(ADPS2);
    
    // More than 6.4 MHz  
    #else
      // Enable the ADC, set the prescaler to 128 --> F_CPU / 128
      ADCSRA |= _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
    #endif
  #endif
}
  
