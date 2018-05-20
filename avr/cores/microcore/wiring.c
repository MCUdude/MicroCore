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
#ifdef ENABLE_MILLIS
uint32_t millis()
{  
  return wdt_interrupt_counter;
}
#endif // ENABLE_MILLIS


/***** MICROS() *****/
// Enabling micros() will cause the processor to interrupt more often (every 2048th clock cycle if 
// F_CPU < 4.8 MHz, every 16384th clock cycle if F_CPU >= 4.8 MHz. This will add some overhead when F_CPU is
// less than 4.8 MHz. It's disabled by default because it occupies precious flash space and loads the CPU with
// additional interrupts and calculations. Also note that micros() aren't very precise for frequencies that 64
// doesn't divide evenly by, such as 9.6 and 4.8 MHz.
//Micros not working on Attiny10 and Attiny9 (probably for low memory ram)
#if defined(ENABLE_MICROS) && defined(__AVR_ATtiny13__)
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
  
  #if defined(__AVR_ATtiny10__) || defined(__AVR_ATtiny9__)
    //Set the working clock of the Attiny10 and Attiny9
      CCP = 0xd8;
      CLKPSR = 0x00;
    #if   F_CPU == 8000000
      CCP=0xD8;
      CLKPSR=0;
    #elif F_CPU == 4000000
      CCP=0xD8;
      CLKPSR=1;
    #elif F_CPU == 2000000
      CCP=0xD8;
      CLKPSR=2;
    #elif F_CPU == 1000000
      CCP=0xD8;
      CLKPSR=3;
    #elif F_CPU == 500000
      CCP=0xD8;
      CLKPSR=4;
    #elif F_CPU == 250000
      CCP=0xD8;
      CLKPSR=5;
    #elif F_CPU == 125000
      CCP=0xD8;
      CLKPSR=6;
    #else
      #error CPU SET FREQUENCY ERROR
    #endif
  #endif
  
  #ifdef SETUP_PWM  
    // Set Timer0 prescaler
    #if defined(PWM_PRESCALER_AUTO)
      #if F_CPU >= 4800000L
        TCCR0B = _BV(CS00) | _BV(CS01); // PWM frequency = (F_CPU/256) / 64
      #else  
        TCCR0B = _BV(CS01);             // PWM frequency = (F_CPU/256) / 8
      #endif
    #elif defined(PWM_PRESCALER_NONE)   // PWM frequency = (F_CPU/256) / 1
      TCCR0B = _BV(CS00);
    #elif  defined(PWM_PRESCALER_8)     // PWM frequency = (F_CPU/256) / 8
      TCCR0B = _BV(CS01);
    #elif  defined(PWM_PRESCALER_64)    // PWM frequency = (F_CPU/256) / 64
      TCCR0B = _BV(CS00) | _BV(CS01);
    #elif  defined(PWM_PRESCALER_256)   // PWM frequency = (F_CPU/256) / 256
      TCCR0B = _BV(CS02);
    #elif  defined(PWM_PRESCALER_1024)  // PWM frequency = (F_CPU/256) / 1024
      TCCR0B = _BV(CS00) | _BV(CS02);
    #endif
    
    #if defined(__AVR_ATtiny13__)
      // Set waveform generation mode for Attiny13
      #if defined(PWM_FAST)
        TCCR0A = _BV(WGM00) | _BV(WGM01);
      #elif defined(PWM_PHASE_CORRECT)
        TCCR0A = _BV(WGM00);
      #endif
    #else
      // Set waveform generation mode for Attiny10 and Attiny9
      #if defined(PWM_FAST)			//Fast PWM for ATtiny10 and ATtiny9
				TCCR0B |= _BV(WGM02);		//Fast PWM
				TCCR0A |= _BV(WGM00);
			#elif defined(PWM_PHASE_CORRECT)
				#error PWM_PHASE_CORRECT non implemented on ATtiny10 and ATtiny9
			#endif
    #endif
  #endif  
  
  // Enable WDT interrupt and enable global interrupts  
  #ifdef ENABLE_MILLIS
    // Disable global interrupts      
    cli();
    // Reset watchdog
    wdt_reset();
    #if defined(__AVR_ATtiny13__)
      // Set up WDT interrupt with 16 ms prescaler
      WDTCR = _BV(WDTIE);
    #else
      // Set up WDT of Attiny10 and Attiny9 interrupt with 16 ms prescaler
      WDTCSR = _BV(WDIE);
    #endif
    // Set up WDT interrupt with 16 ms prescaler
    WDTCR = _BV(WDTIE);
    // Enable global interrupts
    sei();
  #endif
  
  // WARNING! Enabling micros() will affect timing functions!
  //Micros not working on Attiny10 and Attiny9 (probably for low memory ram)
  #if defined(ENABLE_MICROS) && defined(__AVR_ATtiny13__)
    
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
    // Turn on global interrupts
    sei();
  #endif 
}
