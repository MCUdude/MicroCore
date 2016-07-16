/*** MicroCore - main.cpp ***
An Arduino core designed for ATtiny13
Based on the work done by "smeezekitty" 
Modified and maintained by MCUdude
*/
  
#include "Arduino.h"
int main(void)
{
	init();  // Call init routine (wiring.c)
	setup(); // Call user provided setup()
	while(1) // Continuiously call user provided loop()
	{
		loop(); 
	}
}
