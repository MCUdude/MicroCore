/*** Core13 - wiring_pulse.c ***
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
//Untested code lurks ahead! If it does or does not work, please notify me!
unsigned long pulseIn(unsigned char pin, unsigned char stat, unsigned long timeout = 0xFFFFFFFF)
{
	unsigned long st,to;
	to = micros();
	while(digitalRead(pin) == stat){if((micros() - to) > timeout){return 0;}}
	while(digitalRead(pin) != stat){if((micros() - to) > timeout){return 0;}}
	st = micros();
	while(digitalRead(pin) == stat){if((micros() - to) > timeout){return 0;}}
	return micros()-st;
}
	
