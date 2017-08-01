/*** MicroCore - WMath.cpp ***
An Arduino core designed for ATtiny13
Based on the work done by "smeezekitty" 
Modified and maintained by MCUdude
https://github.com/MCUdude/MicroCore

This file contains wrapper functions
from stdlib
*/

extern "C" {
  #include <stdlib.h>
  #include <stdint.h>
}


void randomSeed(uint16_t seed)
{
  if (seed != 0) 
    srandom(seed);
}


int32_t random(int32_t howbig)
{
  if (howbig == 0) 
    return 0;
  
  return random() % howbig;
}


int32_t random(int32_t howsmall, int32_t howbig)
{
  if (howsmall >= howbig) 
    return howsmall;
  
  int32_t diff = howbig - howsmall;
  return random(diff) + howsmall;
}


uint16_t makeWord(uint16_t w) 
{ 
  return w; 
}


uint16_t makeWord(uint8_t h, uint8_t l) 
{ 
  return (h << 8) | l; 
}
