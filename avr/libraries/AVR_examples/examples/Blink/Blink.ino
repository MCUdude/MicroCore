/***************************************************
 Simple AVR sketch to blink an LED (or similar)
 connected to PB1 (Arduino pin D1)
 Note that the compiled sketch is significally
 smaller that the original Arduino blink sketch.
 A detailed explaination about debugging, port and
 pin definitions can be watched here:
 https://www.youtube.com/watch?v=plJf0r7IcWc
***************************************************/ 


#include <avr/io.h>
#include <util/delay.h>


int main(void)
{       

  /************************************************ 
   A few different ways to set a pin as output and 
   ignore the rest of the pins. Pick your favorite!
  *************************************************/
  DDRB |= 0x01; 		// XXXXXXXX | 00000010 = XXXXXX1X
  //DDRB |= _BV(PB1);	 // Set PB1 as output, ignore the rest
  //DDRB |= (1 << PB1); // Shift the number '1' left 'PB1' times (PB1 = 2)



  /************************************************ 
   A few different ways to set a pin as input and 
   ignore the rest of the pins.
  *************************************************/
  //DDRB &= ~0x01; //00000010 -> 11111101 & XXXXXXXX = XXXXXX0X
  //DDRB &= ~_BV(PB1); 
  //DDRB &= ~(1 << PB1);
    
  
  // Infinite loop
  while(1)
  {
    /*********************************************** 
     A few different ways to set a pin high and 
     ignore the rest of the pins. 
    ************************************************/
    PORTB |= 0x01;  // XXXXXXXX | 00000010 = XXXXXX1X
    //PORTB |= _BV(PB1);
    //PORTB |= (1 << PB1);
	  //SetPinHigh(&PORTB, 1); 
    _delay_ms(1000);
    
    
    /*********************************************** 
     A few different ways to set a pin low and 
     ignore the rest of the pins.
    ************************************************/
    PORTB &= ~0x01;  // 00000010 -> 11111101 & XXXXXXXX = XXXXXX0X
    //PORTB &= ~_BV(PB1);
    //PORTB &= ~(1 << PB1);
    //SetPinLow(&PORTB, 1);
    _delay_ms(1000);
  }
    
  return 0;
}


// Method to set a pin HIGH
void SetPinHigh(volatile byte *port, byte pin)
{
  *port |= (1 << pin); // The number '1' are shifted left 'pin' times
}

// Method to set pin LOW
void SetPinLow(volatile byte *port, byte pin)
{
  *port &= ~(1 << pin); // The number '1' are shifted left 'pin' times
}
