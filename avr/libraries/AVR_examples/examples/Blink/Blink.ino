/***************************************************
 Simple AVR sketch to blink an LED (or similar)
 connected to PB3 (Arduino pin D3)
 Note that the compiled sketch is significantly
 smaller than the original Arduino blink sketch.
 A detailed explanation about debugging, port and
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
  DDRB |= 0x08;     // XXXXXXXX | 00000100 = XXXXX1XX
  //DDRB |= _BV(PB3);  // Set PB1 as output, ignore the rest
  //DDRB |= (1 << PB3); // Shift the number '1' left 'PB1' times (PB1 = 2)



  /************************************************ 
   A few different ways to set a pin as input and 
   ignore the rest of the pins.
  *************************************************/
  //DDRB &= ~0x08; //00000100 -> 11111011 & XXXXXXXX = XXXXX0XX
  //DDRB &= ~_BV(PB3); 
  //DDRB &= ~(1 << PB3);
    
  
  // Infinite loop
  while(1)
  {
    /*********************************************** 
     A few different ways to set a pin high and 
     ignore the rest of the pins. 
    ************************************************/
    PORTB |= 0x08;  // XXXXXXXX | 00000100 = XXXXX1XX
    //PORTB |= _BV(PB3);
    //PORTB |= (1 << PB3);
    //SetPinHigh(&PORTB, 3); 
    _delay_ms(1000);
    
    
    /*********************************************** 
     A few different ways to set a pin low and 
     ignore the rest of the pins.
    ************************************************/
    PORTB &= ~0x08;  // 00000100 -> 11111001 & XXXXXXXX = XXXXX0XX
    //PORTB &= ~_BV(PB3);
    //PORTB &= ~(1 << PB3);
    //SetPinLow(&PORTB, 3);
    _delay_ms(1000);
  }
    
  return 0;
}



/************************************************ 
  Here are two custom functions to set a pin 
  high or low. The "port pointer" is well
  explained in the video linked in the header,
  and the pointer is basically there to make sure
  it's the actual port and not it's value you're
  passing to the function.
*************************************************/

// Function to set a pin HIGH
void SetPinHigh(volatile byte *port, byte pin)
{
  *port |= (1 << pin); // The number '1' are shifted left 'pin' times
  //*port |= _BV(pin); // Alternative method
}

// Function to set pin LOW
void SetPinLow(volatile byte *port, byte pin)
{
  *port &= ~(1 << pin); // The number '1' are shifted left 'pin' times
  //*port &= ~_BV(pin); // Alternative method
}
