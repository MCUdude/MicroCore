/*** MicroCore - Arduino.h ***
An Arduino core designed for ATtiny13
Based on the work done by "smeezekitty" 

Modified and maintained by MCUdude
https://github.com/MCUdude/MicroCore
*/  
                
#ifndef Arduino_h
#define Arduino_h

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/wdt.h>
#include "WString.h"


#ifdef __cplusplus
extern "C"{	
#endif
  #define HIGH 1
  #define LOW 0
  #define INPUT 0
  #define OUTPUT 1
  #define INPUT_PULLUP 2
  #define FALSE 0
  #define TRUE 1
  #define PI 3.1416
  #define HALF_PI 1.57
  #define TWO_PI 6.283
  #define DEG_TO_RAD 0.0174533
  #define RAD_TO_DEG 57.2958
  #define SERIAL 0
  #define DISPLAY 1 
  #define LSBFIRST 0
  #define MSBFIRST 1
  #define CHANGE 1
  #define FALLING 2
  #define RISING 3
  #define INTERNAL 1
  #define EXTERNAL 0
  #define DEFAULT 0
  #define NOT_AN_INTERRUPT -1
  #ifdef abs
    #undef abs
  #endif
  #define min(a,b) ((a)<(b)?(a):(b))
  #define max(a,b) ((a)>(b)?(a):(b))
  #define abs(x) ((x)>0?(x):-(x))
  #define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
  #define round(x)     ((x)>=0?(int32_t)((x)+0.5):(int32_t)((x)-0.5))
  #define radians(deg) ((deg)*DEG_TO_RAD)
  #define degrees(rad) ((rad)*RAD_TO_DEG)
  #define sq(x) ((x)*(x))
  #define interrupts() sei()
  #define noInterrupts() cli()
  #define clockCyclesPerMicrosecond() ( F_CPU / 1000000L )
  #define clockCyclesToMicroseconds(a) ( ((a) * 1000L) / (F_CPU / 1000L) )
  #define microsecondsToClockCycles(a) ( ((a) * (F_CPU / 1000L)) / 1000L )
  #define lowByte(w) ((uint8_t) ((w) & 0xff))
  #define highByte(w) ((uint8_t) ((w) >> 8))
  #define bitRead(value, bit) (((value) >> (bit)) & 0x01)
  #define bitSet(value, bit) ((value) |= (1UL << (bit)))
  #define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
  #define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))
  #define delayMicroseconds(us) _delay_us(us) // _delay_us function wrapper
  #define bit(b) (1UL << (b))
  #define digitalPinToInterrupt(p) ((p) == 1 ? 0 : NOT_AN_INTERRUPT)
  typedef uint16_t word;
  typedef uint8_t byte;
  typedef bool boolean;
  void init(void);
  void pinMode(uint8_t, uint8_t);
  void digitalWrite(uint8_t, uint8_t);
  void delay(uint16_t);
  uint8_t digitalRead(uint8_t);
  int16_t analogRead(uint8_t); 
  void analogReference(uint8_t mode);
  void analogWrite(uint8_t, uint8_t);
  uint32_t millis(void);
  uint32_t micros(void);
  void shiftOut(uint8_t, uint8_t, uint8_t, uint8_t);
  uint8_t shiftIn(uint8_t, uint8_t, uint8_t);
  uint32_t pulseIn(uint8_t pin, uint8_t stat, uint32_t timeout);
  void attachInterrupt(uint8_t, void (*)(void), uint8_t mode);
  void detachInterrupt(uint8_t);
  void setup(void);
  void loop(void);
#ifdef __cplusplus
} // extern "C"

// Pin definitions
#define LED_BUILTIN 0
const static uint8_t A0 = 5;
const static uint8_t A1 = 2;
const static uint8_t A2 = 4;
const static uint8_t A3 = 3;

#ifdef __cplusplus
  uint16_t makeWord(uint16_t w);
  uint16_t makeWord(uint8_t h, uint8_t l);
  #define word(...) makeWord(__VA_ARGS__)
  int32_t random(int32_t);
  int32_t random(int32_t, int32_t);
  void randomSeed(uint16_t);
  int32_t map(int32_t, int32_t, int32_t, int32_t, int32_t);
#endif

#endif

#endif
