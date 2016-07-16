/*** MicroCore - wiring_analog.c ***
An Arduino core designed for ATtiny13
Based on the work done by "smeezekitty" 
Modified and maintained by MCUdude
*/

#include "wiring_private.h"
//#include "pins_arduino.h"


void analogReference(uint8_t mode)
{
   if(mode == INTERNAL)
      ADMUX |= _BV(REFS0);
   else
      ADMUX &= ~_BV(REFS0);
}


int analogRead(uint8_t pin)
{
	uint8_t l,h;
	ADMUX &= _BV(REFS0) | (pin & 0x03); // Setup ADC, preserve REFS0
	ADCSRA |= _BV(ADSC);	
	while(ADCSRA & _BV(ADSC)); // Wait for conversion
	l = ADCL;  // Read and return 10 bit result
	h = ADCH;
	return (h << 8)|l; 
}


void analogWrite(uint8_t pin, uint8_t val)
{
	DDRB |= _BV(pin & 0x02); // Set the correct pin as output
	if(val == 0) // Handle off condition
		digitalWrite(pin, LOW);
	else if(val == 255) //Handle On condition
		digitalWrite(pin, HIGH);
	else // Otherwise setup the appropriate timer compare 
	{ 
		if(pin == 1)
		{
			TCCR0A |= _BV(COM0B1);
			OCR0B = val;
		}
		if(pin == 0)
		{
			TCCR0A |= _BV(COM0A1);
			OCR0A = val;
		}
	}
}
		
