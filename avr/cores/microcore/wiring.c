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
// Enabling micros() will cause the processor to interrupt more often (every 2048th clock cycle if 
// F_CPU < 4.8 MHz, every 16384th clock cycle if F_CPU >= 4.8 MHz. This will add some overhead when F_CPU is
// less than 4.8 MHz. It's disabled by default because it occupies precious flash space and loads the CPU with
// additional interrupts and calculations. Also note that micros() aren't very precise for frequencies that 64
// doesn't divide evenly by, such as 9.6 and 4.8 MHz.
#ifdef ENABLE_MICROS
volatile uint32_t timer0_overflow = 0;

// This will cause an interrupt every 256*64 clock cycle
ISR(TIM0_OVF_vect)
{
  timer0_overflow++; // Increment counter by one
}

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
  #elif F_CPU == 1200000L
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
 #endif

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
    #if defined(PWM_PRESCALER_DEFAULT)
      #if F_CPU >= 4800000L
        TCCR0B |= _BV(CS00) | _BV(CS01); // PWM frequency = (F_CPU/256) / 64
      #else  
        TCCR0B |= _BV(CS01);             // PWM frequency = (F_CPU/256) / 8
      #endif
    #elif defined(PWM_PRESCALER_NONE)    // PWM frequency = (F_CPU/256) / 1
      TCCR0B |= _BV(CS00);
    #elif  defined(PWM_PRESCALER_8)      // PWM frequency = (F_CPU/256) / 8
      TCCR0B |= _BV(CS01);
    #elif  defined(PWM_PRESCALER_64)     // PWM frequency = (F_CPU/256) / 64
      TCCR0B |= _BV(CS00) | _BV(CS01);
    #elif  defined(PWM_PRESCALER_256)    // PWM frequency = (F_CPU/256) / 256
      TCCR0B |= _BV(CS02);
    #elif  defined(PWM_PRESCALER_1024)   // PWM frequency = (F_CPU/256) / 1024
      TCCR0B |= _BV(CS00) | _BV(CS02);
    #endif
    
    // Set waveform generation mode
    #if defined(PWM_FAST)
      TCCR0A |= _BV(WGM00) | _BV(WGM01);
    #elif defined(PWM_PHASE_CORRECT)
      TCCR0A |= _BV(WGM00);
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
  
  // WARNING! Enabling micros() will affect timing functions!
  #ifdef ENABLE_MICROS
    
    // Clear all Timer0 settings
    TCCR0B = 0;
    // Set a suited prescaler based on F_CPU
    #if F_CPU >= 4800000L
      TCCR0B |= _BV(CS00) | _BV(CS01); // F_CPU/64 
    #else  
      TCCR0B |= _BV(CS01);             // F_CPU/8
    #endif      
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
    
    // Default ADC prescaler option
    #ifdef ADC_PRESCALER_DEFAULT
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
      
    // ADC prescaler 2   
    #elif defined(ADC_PRESCALER_2)   
      ADCSRA |= _BV(ADEN);     
    // ADC prescaler 4
    #elif defined(ADC_PRESCALER_4)
      ADCSRA |= _BV(ADEN) | _BV(ADPS1);       
    // ADC prescaler 8
    #elif defined(ADC_PRESCALER_8)
      ADCSRA |= _BV(ADEN) | _BV(ADPS1) | _BV(ADPS0);     
    // ADC prescaler 16
    #elif defined(ADC_PRESCALER_16)
      ADCSRA |= _BV(ADEN) | _BV(ADPS2);
    // ADC prescaler 32
    #elif defined(ADC_PRESCALER_32)
      ADCSRA |= _BV(ADEN) | _BV(ADPS2) | _BV(ADPS0);
    // ADC prescaler 64
    #elif defined(ADC_PRESCALER_64)
      ADCSRA |= _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1);
    // ADC prescaler 128
    #elif defined(ADC_PRESCALER_128)
      ADCSRA |= _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);  
    #endif
  #endif
}
  
