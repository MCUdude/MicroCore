/*** MicroCore - wiring_private.c ***
An Arduino core designed for ATtiny13
Based on the work done by "smeezekitty" 
Modified and maintained by MCUdude
https://github.com/MCUdude/MicroCore
This file contains low level macros.
*/

#ifndef WiringPrivate_h
#define WiringPrivate_h

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdarg.h>

#include "Arduino.h"

#ifdef __cplusplus
extern "C"{
#endif

#ifndef cbi
  #define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
  #define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

typedef void (*voidFuncPtr)(void);

#ifdef __cplusplus
} // extern "C"
#endif

#endif