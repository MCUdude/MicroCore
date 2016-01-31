/*
*** Core13 ***
Arduino core designed for Attiny13 and similar devices.
NO WARRANTEE OR GUARANTEES!
Written by John "smeezekitty" 
You are free to use, redistribute and modify at will EXCEPT IF MARKED OTHERWISE IN A PARTICULAR SOURCE FILE!
Version 0.22
// ATMEL ATTINY13 - ARDUINO
//
//                  +-\/-+
// AinX (D X) PB5  1|    |8  Vcc
// AinX (D X) PB3  2|    |7  PB2 (D X)  AinX
// AinX (D X) PB4  3|    |6  PB1 (D X) PWM
//            GND  4|    |5  PB0 (D X) PWM
//                  +----+
*/
#ifndef Wiring_h
#define Wiring_h
#include <avr/io.h>
#include <stdlib.h>
#ifdef __cplusplus
extern "C"{
#endif
#define HIGH 1
#define LOW 0
#define INPUT 0
#define OUTPUT 1
#define false 0
#define FALSE 0
#define TRUE 1
#define true 1
#define PI 3.1416
#define HALF_PI 1.57
#define TWO_PI 6.283
#define DEG_TO_RAD 0.0174533
#define RAD_TO_DEG 57.2958
#define SERIAL 0
#define DISPLAY 1 //WTF are these?
#define LSBFIRST 0
#define MSBFIRST 1
#define CHANGE 1
#define FALLING 2
#define RISING 3
#define INTERNAL 1
#define EXTERNAL 0
#define DEFAULT 0
#ifdef abs
#undef abs
#endif
#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#define abs(x) ((x)>0?(x):-(x))
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define round(x)     ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
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
typedef unsigned int word;
#define bit(b) (1UL << (b))
typedef uint8_t boolean;
typedef uint8_t byte;
void init(void);
void pinMode(uint8_t, uint8_t);
void digitalWrite(uint8_t, uint8_t);
uint8_t digitalRead(uint8_t);
int analogRead(uint8_t); 
void analogReference(uint8_t mode);
void analogWrite(uint8_t, uint8_t);
unsigned long millis(void);
unsigned long micros(void);
void delay(unsigned ms);
void delayMicroseconds(int us);
void shiftOut(uint8_t, uint8_t, uint8_t, uint8_t);
uint8_t shiftIn(uint8_t, uint8_t, uint8_t);
unsigned long pulseIn(unsigned char pin, unsigned char stat, unsigned long timeout);
//void attachInterrupt(uint8_t, void (*)(void), int mode);
//void detachInterrupt(uint8_t);
void setup(void);
void loop(void);
#ifdef __cplusplus
} // extern "C"
#endif
#endif
