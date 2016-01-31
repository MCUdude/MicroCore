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
//Untested code lurks ahead! If it does or does not work, please notify me!
unsigned long pulseIn(unsigned char pin, unsigned char stat, unsigned long timeout = 0xFFFFFFFF){
	unsigned long st,to;
	to = micros();
	while(digitalRead(pin) == stat){if((micros() - to) > timeout){return 0;}}
	while(digitalRead(pin) != stat){if((micros() - to) > timeout){return 0;}}
	st = micros();
	while(digitalRead(pin) == stat){if((micros() - to) > timeout){return 0;}}
	return micros()-st;
}
	