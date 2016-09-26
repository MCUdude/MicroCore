/********************************************************* 
 This sketch demonstrates the use of the watchdog timer
 interrupt service routine. The WDT can be setup to cause
 an interrupt instead of resetting the microcontroller.
 This is how millis() works in this core.

 The WDT will cause an interrupt every 500 ms and toggles
 PB2 every time there's an interrupt.
 ********************************************************/ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

int main(void)
{
  cli(); // Turn off global interrupts

  DDRB |= _BV(PB2); // Set PB2 as output
  
  wdt_reset(); // Reset watchdog

  // Turn on WDT interrupt and cause an interrupt every 500 ms
  // Read more about how to change the interrupt delay on page
  // 42 in the ATtiny13 datasheet
  // http://www.atmel.com/images/doc2535.pdf
  WDTCR = _BV(WDTIE) | _BV(WDP2) | _BV(WDP0); 

  sei(); // Turn on global interrupts
  
  while(1); // Wait forever
}


// This will run every time the WDT causes an interrupt
ISR(WDT_vect)
{
  // Toggle PB2
  PORTB ^= _BV(PB2);
}

