/*** MicroCore - main.cpp ***
An Arduino core designed for ATtiny13
Based on the work done by "smeezekitty" 
Modified and maintained by MCUdude
https://github.com/MCUdude/MicroCore

This is the main program where the
initializing, setup and loop function
runs.
*/
  
#include "Arduino.h"

int main(void)
{
  init();  // Call init routine (wiring.c)
  setup(); // Call user provided setup()
  while(1) // Continuously call user provided loop()
  {
    loop(); 
  }
}
