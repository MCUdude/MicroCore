/*** MicroCore - wiring_pulse.c ***
An Arduino core designed for ATtiny13
Based on the work done by "smeezekitty" 
Modified and maintained by MCUdude
*/

#include "wiring_private.h"

unsigned long pulseIn(unsigned char pin, unsigned char stat, unsigned long timeout)
{
	unsigned long st,to;
	to = micros();
	while(digitalRead(pin) == stat){if((micros() - to) > timeout){return 0;}}
	while(digitalRead(pin) != stat){if((micros() - to) > timeout){return 0;}}
	st = micros();
	while(digitalRead(pin) == stat){if((micros() - to) > timeout){return 0;}}
	return micros()-st;
}
	
