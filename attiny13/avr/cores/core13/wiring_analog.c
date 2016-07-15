/*** Core13 - wiring_analog.c ***
Arduino core designed for Attiny13 and similar devices.
NO WARRANTEE OR GUARANTEES!
Written by John "smeezekitty" 
Modified by MCUdude
You are free to use, redistribute and modify at will EXCEPT IF MARKED OTHERWISE IN A PARTICULAR SOURCE FILE!
Version v0.3.0

 	   ATTINY13 - ARDUINO
               +-\/-+
 A0 (D4) PB5  1|    |8  Vcc
 A3 (D3) PB3  2|    |7  PB2 (D2)  A2
 A2 (D5) PB4  3|    |6  PB1 (D1) PWM
         GND  4|    |5  PB0 (D0) PWM
               +----+
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
	ADMUX = (ADMUX & _BV(REFS0)) | pin & 0x03; // Setup ADC, preserve REFS0
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
		
