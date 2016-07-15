/*
*** Core13 ***
Arduino core designed for Attiny13 and similar devices.
NO WARRANTEE OR GUARANTEES!
Written by John "smeezekitty" 
You are free to use, redistribute and modify at will EXCEPT IF MARKED OTHERWISE IN A PARTICULAR SOURCE FILE!
Version v0.3.0
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


void pinMode(uint8_t pin, uint8_t mode)
{
	#ifdef SAFEMODE
		if(pin > 5 || pin < 0)
			return;
	#endif
	if(!mode) // Pin as input
		DDRB &= ~_BV(pin);
	else // Pin as output
        DDRB |= _BV(pin);
	}
}


void turnOffPWM(uint8_t timer)
{
	if(timer == 0)
		TCCR0A &= ~_BV(COM0A1);
	if(timer == 1)
		TCCR0A &= ~_BV(COM0B1);
}


void digitalWrite(uint8_t pin, uint8_t val)
{
	#ifdef SAFEMODE
		if(pin > 5 || pin < 0)
			return;
		if(pin < 2)
			turnOffPWM(pin); //If its a PWM pin, make sure the PWM is off
	#endif		
	if(!val)
		PORTB &= ~_BV(pin);
	else
		PORTB |= _BV(pin);
}


uint8_t digitalRead(uint8_t pin)
{
	#ifdef SAFEMODE
		if(pin > 5 || pin < 0)
			return 0;
		if(pin < 2)
			turnOffPWM(pin); //If its PWM pin, makes sure the PWM is off
	#endif		
	return !!(PINB & _BV(pin));
}
