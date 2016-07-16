/*** MicroCore - wiring_shift.c ***
An Arduino core designed for ATtiny13
Based on the work done by "smeezekitty" 
Modified and maintained by MCUdude
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
