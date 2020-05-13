#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "Arduino.h"
#include "core_settings.h"

// I/O register macros
#define GBIT(r,b)    b
#define GPORT(r,b)   (PORT ## r)
#define GDDR(r,b)    (DDR ## r)
#define GPIN(r,b)    (PIN ## r)
#define get_bit(io)  GBIT(io)
#define get_port(io) GPORT(io)
#define get_ddr(io)  GDDR(io)
#define get_pin(io)  GPIN(io)

#define PUTXBIT      get_bit(UART_TX_PIN)
#define PUTXPORT     get_port(UART_TX_PIN)
#define PUTXDDR      get_ddr(UART_TX_PIN)
#define PURXBIT      get_bit(UART_RX_PIN)
#define PURXPIN      get_pin(UART_RX_PIN)

// Set default baud rate based on F_CPU
#ifndef CUSTOM_BAUD_RATE
  #if F_CPU >= 5000000L
    #define BAUD_RATE  115200
  #elif F_CPU >= 4800000L
    #define BAUD_RATE  57600  
  #elif F_CPU >= 1000000L
    #define BAUD_RATE  19200
  #elif F_CPU >= 600000L
    #define BAUD_RATE  9600
  #else
    #define BAUD_RATE  300
    #warning Clock speed too slow for serial communication!
  #endif
#else
  #define BAUD_RATE CUSTOM_BAUD_RATE
#endif

// Make these functions work like type-safe macros
#define MUST_INLINE __attribute((always_inline)) inline

// MUST_INLINE float PUBIT_CYCLES() {return F_CPU/(PU_BAUD_RATE*1.0);}
constexpr float PUBIT_CYCLES = F_CPU/(BAUD_RATE*1.0);

// Delay based on cycle count of asm code + 0.5 for rounding
const int16_t PUTXWAIT = PUBIT_CYCLES - 7 + 0.5;
const int16_t PURXWAIT = PUBIT_CYCLES - 5 + 0.5;

// Correct for PURXWAIT skew in PURXSTART calculation
// Skew is half of 7 delay intervals between 8 bits
const float PUSKEW = ( PUBIT_CYCLES - (int)(PUBIT_CYCLES + 0.5) ) * 3.5;

// Time from falling edge of start bit to sample 1st bit is 1.5 *
// bit-time. Subtract 2c for sbic, 1 for ldi, 1 for lsr, and PURXWAIT.
// Subtract 1.5 cycles because start bit detection is accurate to
// +-1.5 cycles.  Add 0.5 cycles for int rounding, and add skew.
const int16_t PURXSTART = PUBIT_CYCLES*1.5 -5.5 -PURXWAIT + 0.5 + PUSKEW;

extern "C" void putx(uint8_t c);
uint8_t purx();

// PCINT-based Rx functions
// Read purx_data & re-enable ISR
uint8_t pu_read();
uint8_t pu_peek();

// Generic picoUART print function
void prints(const char* s);
void prints(const __FlashStringHelper *ifsh);

// Data is ready when PCINT is disabled
MUST_INLINE bool purx_dataready() { return !(PCMSK & 1<<PURXBIT); } 
