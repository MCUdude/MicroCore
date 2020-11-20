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
#include "binary.h"

// Millis counter variable defined in millis.S
extern volatile uint32_t wdt_interrupt_counter;

// timer0 count variable defined in wiring.c
extern volatile uint32_t timer0_overflow;

// Throw error if LTO is enabled and GCC version is lower than 4.9.2
#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#if GCC_VERSION < 40902
#error Your compiler does not support LTO. Please either upgrade Arduino AVR Boards and/or your IDE
#endif  //GCC_VERSION < 40902

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

  #ifndef abs
    #define abs(x)     __builtin_abs(x)
  #endif
  #define sq(x)        ({ typeof (x) _x = (x); _x * _x; })
  #define min(a,b)     ({ typeof (a) _a = (a); typeof (b) _b = (b); _a < _b ? _a : _b;    })
  #define max(a,b)     ({ typeof (a) _a = (a); typeof (b) _b = (b); _a > _b ? _a : _b;    })
  #define round(x)     ({ typeof (x) _x = (x);  _x >= 0 ? (long)_x + 0.5 : (long)_x - 0.5 })
  #define radians(deg) ((deg) * DEG_TO_RAD)
  #define degrees(rad) ((rad) * RAD_TO_DEG)
  #define constrain(x,low,high)   ({ \
    typeof (x) _x = (x);             \
    typeof (low) _l = (l);           \
    typeof (high) _h = (h);          \
    _x < _l ? _l : _x > _h ? _h :_x })
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
  // main in .init9 saves rjmp instruction
  __attribute__(( section(".init9") )) int main();
  void init(void);
  void pinMode(uint8_t pin, uint8_t mode);
  void digitalWrite(uint8_t pin, uint8_t val);
  void delay(uint16_t ms);
  uint8_t digitalRead(uint8_t pin);
  int16_t analogRead(analog_pin_t pin);
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
  void yield(void) __attribute__ ((weak, alias("__empty")));
  static void __empty() { /* Empty*/ }

  // Constant checks error handler
  void badArg(const char*) __attribute__((error("")));
  #define ASSERT_CONST(pin) \
    if(!__builtin_constant_p(pin)) badArg("Pin must be a constant")

  // Check for valid PWM pin (0 .. 5)
  __attribute__((always_inline))
  inline void check_valid_digital_pin(uint8_t pin)
  {
    if(__builtin_constant_p(pin))
    {
      if(pin >= NUM_DIGITAL_PINS)
        badArg("Digtial pin out of range");
    }
    else
      badArg("Digital pin must be a constant");
  }

  // Check for valid analog pin (A0 .. A3)
  __attribute__((always_inline))
  inline void check_valid_analog_pin(analog_pin_t pin)
  {
    if(__builtin_constant_p(pin))
    {
      if(pin >= BAD_ANALOG_PIN)
        badArg("Analog pin out of range");
    }
    else
      badArg("Analog pin must be a constant");
  }

  // Check for valid PWM pin (0 .. 1)
  __attribute__((always_inline))
  inline void check_valid_pwm_pin(uint8_t pin)
  {
    if(__builtin_constant_p(pin))
    {
      if(pin > 1)
        badArg("PWM pin out of range");
    }
    else
      badArg("PWM pin must be a constant");
  }

#ifdef __cplusplus
} // extern "C"


#ifdef __cplusplus

  #include "HalfDuplexSerial.h"

  // Tone functions
  void tone(uint8_t pin, uint16_t frequency, uint32_t duration = 0);
  void toneRaw(uint8_t pin, uint8_t midPoint, uint32_t lengthTicks, uint8_t prescaleBitMask);
  void noTone(uint8_t pin = 255);
  void stopTone(void);

  uint16_t makeWord(uint16_t w);
  uint16_t makeWord(uint8_t h, uint8_t l);
  #define word(...) makeWord(__VA_ARGS__)

  uint32_t pulseIn(uint8_t pin, uint8_t state, uint32_t timeout = 1000000L);

  // Math functions
  int32_t random(int32_t howbig);
  int32_t random(int32_t howsmall, int32_t howbig);
  void randomSeed(uint16_t seed);
  template <typename A, typename B, typename C, typename D, typename E>
  int32_t map(A in, B in_min, C in_max, D out_min, E out_max)
  {
    return (int32_t)(in - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  }

#endif

#endif

#endif
