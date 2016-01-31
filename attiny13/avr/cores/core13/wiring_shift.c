/*
*** Core13 ***
Arduino core designed for Attiny13 and similar devices.
NO WARRANTEE OR GUARANTEES!
Written by John "smeezekitty" 
You are free to use, redistribute and modify at will EXCEPT IF MARKED OTHERWISE IN A PARTICULAR SOURCE FILE!
Version 0.22
// ATMEL ATTINY13 - ARDUINO
//
//                  +-\/-+
// AinX (D X) PB5  1|    |8  Vcc
// AinX (D X) PB3  2|    |7  PB2 (D X)  AinX
// AinX (D X) PB4  3|    |6  PB1 (D X) PWM
//            GND  4|    |5  PB0 (D X) PWM
//                  +----+
*/
#include "wiring_private.h"
uint8_t shiftIn(uint8_t d, uint8_t c, uint8_t bitor) {
	uint8_t value = 0;
	uint8_t i;
	for (i = 0; i < 8; ++i) {
		digitalWrite(c, HIGH);
	/*	if(bitor == LSBFIRST){value |= digitalRead(d) << i;}
		else{value |= digitalRead(d) << (7 - i);}*/
		value |= (digitalRead(d) << ((bitor == LSBFIRST)?i:7-i));
		digitalWrite(c, LOW);
	}
	return value;
}

void shiftOut(uint8_t d, uint8_t c, uint8_t bitor, uint8_t val)
{
	uint8_t i;
	for (i = 0; i < 8; i++)  {
		if(bitor == LSBFIRST){digitalWrite(d, !!(val & (1 << i)));}
		else{digitalWrite(d, !!(val & (1 << (7 - i))));}
		digitalWrite(c, HIGH);
		asm("nop");
		digitalWrite(c, LOW);		
	}
}
