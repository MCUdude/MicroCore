/*** Core13 - wiring_shift.c ***
Arduino core designed for ATtiny13
NO WARRANTEE OR GUARANTEES!
You are free to use, redistribute and modify at will EXCEPT IF MARKED OTHERWISE IN A PARTICULAR SOURCE FILE!
Written by John "smeezekitty" 
Modified by MCUdude

 	 			ATTINY13 - ARDUINO
               +-\/-+
 A0 (D4) PB5  1|    |8  Vcc
 A3 (D3) PB3  2|    |7  PB2 (D2)  A1
 A2 (D5) PB4  3|    |6  PB1 (D1) PWM
         GND  4|    |5  PB0 (D0) PWM
               +----+
*/

#include "wiring_private.h"
uint8_t shiftIn(uint8_t d, uint8_t c, uint8_t bitor) 
{
	uint8_t value = 0;
	for(uint8_t i = 0; i < 8; ++i) 
	{
		digitalWrite(c, HIGH);
		//if(bitor == LSBFIRST){value |= digitalRead(d) << i;}
		//else{value |= digitalRead(d) << (7 - i);}
		value |= (digitalRead(d) << ((bitor == LSBFIRST) ? i : 7 - i));
		digitalWrite(c, LOW);
	}
	return value;
}

void shiftOut(uint8_t d, uint8_t c, uint8_t bitor, uint8_t val)
{
	for(uint8_t i = 0; i < 8; i++)  
	{
		if(bitor == LSBFIRST)
			digitalWrite(d, !!(val & (1 << i)));
		else
			digitalWrite(d, !!(val & (1 << (7 - i))));
		digitalWrite(c, HIGH);
		asm("nop");
		digitalWrite(c, LOW);		
	}
}
