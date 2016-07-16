/*** Core13 - wiring_private.h ***
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

#ifndef WiringPrivate_h
#define WiringPrivate_h

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>
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
#define EXTERNAL_INT_0 0

#define EXTERNAL_NUM_INTERRUPTS 1

typedef void (*voidFuncPtr)(void);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
