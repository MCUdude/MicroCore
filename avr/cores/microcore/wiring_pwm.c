/*** MicroCore - wiring_PWM.c ***
An Arduino core designed for ATtiny13
File created and maintained by MCUdude
https://github.com/MCUdude/MicroCore

This file contains the PWM function
analogWrite().
*/

#include "wiring_private.h"

void analogWrite(uint8_t pin, uint8_t val)
{
  // SAFEMODE prevents you from inserting a pin number out of range
	#ifdef SAFEMODE
		DDRB |= _BV(pin & 0x02); // Set the correct pin as output
	#endif
		
	if(val == 0) // Handle off condition
		digitalWrite(pin, LOW);
	else if(val == 255) // Handle on condition
		digitalWrite(pin, HIGH);
	else // Otherwise setup the appropriate timer compare 
	{ 
		if(pin == 1)		
		{
			TCCR0A |= _BV(COM0B1);
			OCR0B = val;
		}
		
		// SAFEMODE prevents you from inserting a pin number out of range
		#ifdef SAFEMODE
		else if(pin == 0)
		#else
		else // We're saving a few bytes by not asking if pin = 0
		#endif
		{
			TCCR0A |= _BV(COM0A1);
			OCR0A = val;
		}
	}
}