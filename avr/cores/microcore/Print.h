/*
  Print.h - Base class that provides print() and println()
  Copyright (c) 2008 David A. Mellis.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  Modified 20-11-2010 by B.Cook ...

  Modified by MCUdude for the MicroCore
  https://github.com/MCUdude/MicroCore
*/

#ifndef Print_h
#define Print_h

#include <inttypes.h>
#include <stdio.h> // for size_t
#include "WString.h"
#include "core_settings.h"

#define DEC  ((uint8_t) 10)
#define HEX  ((uint8_t) 16)
#define OCT  ((uint8_t)  8)
#define BIN  ((uint8_t)  2)
#define BYTE ((uint8_t)  0)

// Maximum integer type to be handled.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// For small machines (tiny13 etc) handling long may be too demanding
// so we can now specify that int is the largest type we can handle
//
// In other words, put this in your pins_arduino.h file...
//    #define PRINT_MAX_INT_TYPE PRINT_INT_TYPE_INT
// (or pass in buld.extra_flags=-DPRINT_MAX_INT_TYPE=2)
//
// The possible options are exactly these:
//
// PRINT_INT_TYPE_LONG  (default)
// PRINT_INT_TYPE_INT
// PRINT_INT_TYPE_BYTE
//
//

#define PRINT_INT_TYPE_LONG 1
#define PRINT_INT_TYPE_INT  2
#define PRINT_INT_TYPE_BYTE 3

#ifndef PRINT_MAX_INT_TYPE
  // The official Arduino core can do long, so this is the default
  // if your variant doesn't specify otherwise.
  #define PRINT_MAX_INT_TYPE    PRINT_INT_TYPE_LONG
#endif

#if PRINT_MAX_INT_TYPE == PRINT_INT_TYPE_LONG
  #define PRINT_INT_TYPE            int32_t
  #define UNSIGNED_PRINT_INT_TYPE   uint32_t
  #define PGM_READ_MAX_INT_TYPE(A)  pgm_read_dword(A)
#elif PRINT_MAX_INT_TYPE == PRINT_INT_TYPE_INT
  #define PRINT_INT_TYPE            int16_t
  #define UNSIGNED_PRINT_INT_TYPE   uint16_t
  #define PGM_READ_MAX_INT_TYPE(A)  pgm_read_word(A)
#elif PRINT_MAX_INT_TYPE == PRINT_INT_TYPE_BYTE
  #define PRINT_INT_TYPE            int8_t
  #define UNSIGNED_PRINT_INT_TYPE   uint8_t
  #define PGM_READ_MAX_INT_TYPE(A)  pgm_read_byte(A)
#endif


// Number Base printing support
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Tight memory systems (flash and sram) might need to restrict the number
// bases that can be printed, typically it's unnecessary
// to allow arbitrary bases and doing so really eats your sram (and flash).
//
// Set one or more of the following defines (in pins_arduino.h usually):
//
//   #define PRINT_USE_BASE_BIN
//   #define PRINT_USE_BASE_HEX
//   #define PRINT_USE_BASE_OCT
//   #define PRINT_USE_BASE_DEC
//   #define PRINT_USE_BASE_ARBITRARY
//
// if PRINT_USE_BASE_ARBITRARY is set the other defines are ignored
// and the normal Arduino print functions are in play.
//
// if it is not set then an optimised printNumber() function is used
// without division.
//
// If nothing is set then we use the following as the default:
//
// #define PRINT_USE_BASE_BIN
// #define PRINT_USE_BASE_HEX
// #define PRINT_USE_BASE_OCT
// #define PRINT_USE_BASE_DEC
//
// This gives us almost the same functionality as the standard
// arduino print, because 99% of people are only ever going to need
// base 2/8/10 and 16 in their life (frankly, I would omit 8 as well
// except that ASCIITable example uses it).
//
// If you try and print an "unsupported" base it's not a problem
// simply it will "promote" your request to the highest supported
// base and prefix the printed number with a letter to signal
// (b = binary, o = octal, d = decimal, x = hex)
// which base was used instead of what you requested.
//
// If you really want to use non-standard bases, set PRINT_USE_BASE_ARBITRARY
// but be aware that it will bring in a very large amount of overhead
// (mainly ram usage, not good if you only have 64 bytes of RAM!)
//
// For what it's worth the flash-usage for having only ONE base
// enabled was measured on a sketch as follows...
//
// BIN      = lowest
// OCT      = 6 bytes more than BIN only
// HEX      = 6 bytes more than BIN only
// DEC      = highest - 22 bytes more than BIN
//
// Some combinations relative to BIN only:
// BIN+OCT         = +36 bytes
// BIN+HEX         = +52 bytes
// BIN+HEX+OCT     = +72 bytes
// BIN+DEC         = +94 bytes
// DEC+HEX         = +98 bytes
// BIN+HEX+DEC     = +122 bytes
// BIN+HEX+DEC+OCT = +150 bytes
//
// Short version, if you can live with only one supported base
// for printing numbers, you save a whole pile of memory no matter
// which base you choose, even DEC on it's own doesn't eat much
// more than BIN on it's own, but as soon as you start supporting
// more than one base, you start eating flash.
//
// Of course, if you don't actually use anything that calls
//    print(numberhere, [optionalbase])
// then it's all optimised away by gcc and you don't care.
//

#if ! (defined(PRINT_USE_BASE_BIN) || defined(PRINT_USE_BASE_HEX) || defined(PRINT_USE_BASE_OCT) || defined(PRINT_USE_BASE_DEC) || defined(PRINT_USE_BASE_ARBITRARY))
  #define PRINT_USE_BASE_BIN
  #define PRINT_USE_BASE_HEX
  #define PRINT_USE_BASE_OCT
  #define PRINT_USE_BASE_DEC
#endif

class Print
{
  private:
    int write_error;

    size_t printNumber(UNSIGNED_PRINT_INT_TYPE, uint8_t);
    size_t printFloat(double, uint8_t);
  protected:
    void setWriteError(int err = 1) { write_error = err; }
  public:
    Print() : write_error(0) {}

    int getWriteError() { return write_error; }
    void clearWriteError() { setWriteError(0); }

    virtual size_t write(uint8_t) = 0;
    size_t write(const char *str) { return write((const uint8_t *)str, strlen(str)); }
    virtual size_t write(const uint8_t *buffer, size_t size);

    size_t print(const __FlashStringHelper *);
    size_t print(const String &);
    size_t print(const char[]);
    size_t print(char);

    // by using these conditionals we can cut-out some pointless
    // function calls and casting when our "primary" integer type
    // for print is the same as that particular function handles
    // otherwise these guarded functions will cast to the
    // primary type (PRINT_INT_TYPE or UNSIGNED_PRINT_INT_TYPE
    // as appropriate).
    #if PRINT_MAX_INT_TYPE != PRINT_INT_TYPE_BYTE
    size_t print(unsigned char, uint8_t = DEC);
    #endif
    #if PRINT_MAX_INT_TYPE != PRINT_INT_TYPE_INT
    size_t print(int, uint8_t = DEC);
    size_t print(unsigned int, uint8_t = DEC);
    #endif
    #if PRINT_MAX_INT_TYPE != PRINT_INT_TYPE_LONG
    size_t print(long, uint8_t = DEC);
    size_t print(unsigned long, uint8_t = DEC);
    #endif
    size_t print(PRINT_INT_TYPE, uint8_t = DEC);
    size_t print(UNSIGNED_PRINT_INT_TYPE, uint8_t = DEC);
    size_t print(double, uint8_t = 2);

    size_t println(const __FlashStringHelper *);
    size_t println(const String &s);
    size_t println(const char[]);
    size_t println(char);
    #if PRINT_MAX_INT_TYPE != PRINT_INT_TYPE_BYTE
    size_t println(unsigned char, uint8_t = DEC);
    #endif
    #if PRINT_MAX_INT_TYPE != PRINT_INT_TYPE_INT
    size_t println(int, uint8_t = DEC);
    size_t println(unsigned int, uint8_t = DEC);
    #endif
    #if PRINT_MAX_INT_TYPE != PRINT_INT_TYPE_LONG
    size_t println(long, uint8_t = DEC);
    size_t println(unsigned long, uint8_t = DEC);
    #endif
    size_t println(PRINT_INT_TYPE, uint8_t = DEC);
    size_t println(UNSIGNED_PRINT_INT_TYPE, uint8_t = DEC);
    size_t println(double, int = 2);
    size_t println(void);
};

#endif
