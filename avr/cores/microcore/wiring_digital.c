/*** MicroCore - wiring_digital.c ***
An Arduino core designed for ATtiny13
Based on the work done by "smeezekitty" 
Modified and maintained by MCUdude
*/

#include "wiring_private.h"
#include "core_settings.h"

void pinMode(uint8_t pin, uint8_t mode)
{
	#ifdef SAFEMODE
		if(pin > 5)
			return;
	#endif
	if(!mode) // Pin as input
		DDRB &= ~_BV(pin);
	else // Pin as output
    DDRB |= _BV(pin);
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
	#if defined(SAFEMODE)
		if(pin > 5)
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
		if(pin > 5)
			return 0;
		if(pin < 2)
			turnOffPWM(pin); //If its PWM pin, makes sure the PWM is off
	#endif		
	return !!(PINB & _BV(pin));
}
