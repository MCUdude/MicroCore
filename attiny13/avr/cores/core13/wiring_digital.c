/*** Core13 - wiring_digital.c ***
Arduino core designed for Attiny13 and similar devices.
NO WARRANTEE OR GUARANTEES!
Written by John "smeezekitty" 
Modified by MCUdude
You are free to use, redistribute and modify at will EXCEPT IF MARKED OTHERWISE IN A PARTICULAR SOURCE FILE!
Version v0.3.0

 	   ATTINY13 - ARDUINO
                  +-\/-+
 AinX (D 4) PB5  1|    |8  Vcc
 AinX (D 3) PB3  2|    |7  PB2 (D 2)  AinX
 AinX (D 5) PB4  3|    |6  PB1 (D 1) PWM
            GND  4|    |5  PB0 (D 0) PWM
                  +----+
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
