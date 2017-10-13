/************************************************************* 
 This sketch makes the pin PB2 (digital pin 2) toggle every
 second (internal oscillator running at 9.6 MHz). It uses Timer0
 or Timer0B, and divide the clock frequncy by 1024.
 The divided frequencies period is multiplied with the
 number of counts Timer0/Timer0B can take before it overflows.
 The number is then multiplied by 37, and gives approximately
 1 second.  

 9.6MHz / 1024 = 9370 Hz        We divide the 9.6 MHz clock by 1024
 1/9370 = 0.0001067s            Period time
 256 * 0.0001067 = 0.027315    
 0.027315 * 37 = 1.01065 ≈ 1s   
 X = 1.01065 / 0.027315 where X is approximately 37
**************************************************************/ 


#include <avr/io.h>

// Variable to store the time count (uint8_t = byte)
uint8_t timeCount = 0;

int main (void)
{
  DDRB |= _BV(PB2); //Set PB2 as output, ignore the rest

  TCCR0B = _BV(CS02) | _BV(CS00); // clock frequency / 1024 
  OCR0B = 0x00;  // Output compare
  TCNT0 = 0; // Set counter 0 to zero
  TIMSK0 = _BV(TOIE0); // Enable overflow interrupt
  
  sei(); //Enable global interrupts
  
  while(1); // Infinite loop    
}


ISR(TIM0_OVF_vect) //Timer 0 overflow vector - this runs every time timer0 overflows
{
  timeCount++;
  if(timeCount == 37) //Timer overflown for the 37th time
  {
    PORTB ^= _BV(PB2); //toggle PB2
    timeCount = 0;
  }
}