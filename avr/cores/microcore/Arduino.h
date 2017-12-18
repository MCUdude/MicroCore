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
#include "pins_arduino.h"

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
  #define DEFAULT 0
  #define NOT_AN_INTERRUPT -1
  #define NOT_ON_TIMER 0
  #define NOT_A_PIN 0
  #define TIMER0A 1
  #define TIMER0B 2
  #ifdef abs
    #undef abs
  #endif
  #define min(a,b) ((a)<(b)?(a):(b))
  #define max(a,b) ((a)>(b)?(a):(b))
  #define abs(x)   ((x)>0?(x):-(x))
  #define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
  #define round(x)     ((x)>=0?(int32_t)((x)+0.5):(int32_t)((x)-0.5))
  #define radians(deg) ((deg)*DEG_TO_RAD)
  #define degrees(rad) ((rad)*RAD_TO_DEG)
  #define sq(x)        ((x)*(x))
  #define interrupts() sei()
  #define noInterrupts() cli()
  #define clockCyclesPerMicrosecond()  ( F_CPU / 1000000L )
  #define clockCyclesToMicroseconds(a) ( ((a) * 1000L) / (F_CPU / 1000L) )
  #define microsecondsToClockCycles(a) ( ((a) * (F_CPU / 1000L)) / 1000L )
  #define lowByte(w)  ((uint8_t) ((w) & 0xff))
  #define highByte(w) ((uint8_t) ((w) >> 8))
  #define bitRead(value, bit)  (((value) >> (bit)) & 0x01)
  #define bitSet(value, bit)   ((value) |= (1UL << (bit)))
  #define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
  #define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))
  #define delayMicroseconds(us) _delay_us(us) // _delay_us function wrapper
  #define bit(b) (1UL << (b))
  typedef uint16_t word;
  typedef uint8_t byte;
  typedef bool boolean;
  void init(void);
  void pinMode(uint8_t pin, uint8_t mode);
  void digitalWrite(uint8_t pin, uint8_t val);
  void delay(uint16_t ms);
  uint8_t digitalRead(uint8_t pin);
  int16_t analogRead(uint8_t pin); 
  void analogReference(uint8_t mode);
  void turnOffPWM(uint8_t pin);
  void analogWrite(uint8_t pin, uint8_t val);
  uint32_t millis(void);
  uint32_t micros(void);
  void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val);
  uint8_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder);
  uint32_t pulseIn(uint8_t pin, uint8_t state, uint32_t timeout);
  void attachInterrupt(uint8_t interruptNum, void (*)(void), uint8_t mode);
  void detachInterrupt(uint8_t interruptNum);
  void setup(void);
  void loop(void);
  
#ifdef __cplusplus
} // extern "C"



#ifdef __cplusplus
  // Tone functions
  void tone(uint8_t pin, uint16_t frequency, uint32_t duration = 0);
  void toneRaw(uint8_t pin, uint8_t midPoint, uint32_t lengthTicks, uint8_t prescaleBitMask);
  void noTone(uint8_t pin = 255);
  void stopTone(void);
 
  uint16_t makeWord(uint16_t w);
  uint16_t makeWord(uint8_t h, uint8_t l);
  #define word(...) makeWord(__VA_ARGS__)
  
  // Math functions
  int32_t random(int32_t howbig);
  int32_t random(int32_t howsmall, int32_t howbig);
  void randomSeed(uint16_t seed);
  template <typename A, typename B, typename C, typename D, typename E> 
  int32_t map(A x, B in_min, C in_max, D out_min, E out_max)
  {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  }
  
#endif

#endif

#endif
