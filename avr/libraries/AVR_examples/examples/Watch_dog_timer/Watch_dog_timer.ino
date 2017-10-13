/********************************************************* 
 This sketch demonstrates the use of a watchdog timer.
 The watchdog timer causes a processor reset to occur if
 it is allowed to time out. When the sketch is running
 inside the second while loop, the watch dog timer is
 consistently reset to prevent a timeout. When PB0 is 
 pulled high, it breaks out of the while loop and causes 
 the watch dog timer to reset the processor.
 ********************************************************/ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/wdt.h>

// Variable to store the counted value (uint16_t = unsigned int)
uint16_t counter = 0;

int main(void)
{
 
  DDRB |= _BV(PB2);	 		// Set PB2 as output, ignore the rest
  //DDRB |= 0x04; 		  // Alternative method
  //DDRB |= (1 << PB2); // Alternative method

	DDRB &= _BV(PB0);	    // Set PB0 as input, ignore the rest
  //DDRB &= ~0x01; 		  // Alternative method
  //DDRB &= ~(1 << PB0);// Alternative method
  
  PORTB |= _BV(PB0);    // Enable pullup on PB0

  
  while((PINB & _BV(PB0)) == 1) // Wait for PB0 to be pulled down
  {
    PORTB ^= _BV(PB2); //Toggle PB2 while waiting
    _delay_ms(100);
  }


  wdt_enable(WDTO_1S); // Enable WDT with 1 second timeout

  while((PINB & _BV(PB0)) == 0) // Wait for PB0 to be pulled high
  {
    wdt_reset(); // Reset WDT while PB0 are pulled low
    
    counter++;

    if(counter >= 50000)
    {
      PORTB ^= _BV(PB2); // Toggle PB2 every 50000th time
      counter = 0;
    }
  }

  while(1); // The watchdog will reset the microcontroller after 1 second
}

