/*** Core13 - main.cpp ***
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
