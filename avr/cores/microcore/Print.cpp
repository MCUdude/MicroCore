/*
 Print.cpp - Base class that provides print() and println()
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

 Modified 23 November 2006 by David A. Mellis

 Modified by MCUdude for the MicroCore
 https://github.com/MCUdude/MicroCore
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <avr/pgmspace.h>
#include "Arduino.h"
#include "Print.h"

// Public Methods //////////////////////////////////////////////////////////////

/* default implementation: may be overridden */
size_t Print::write(const uint8_t *buffer, size_t size)
{
  // Assume that write always returns 1 for a single character
  // we remove some additions and save a few bytes.
  //
  // Note that Hardware, Software and Basic serial all return 1
  // for character write regardless, so this as a valid assumption
  // at least in those cases.
  //
  // J.Sleeman (sparks@gogo.co.nz)

  for(size_t n = 0; n < size; n++)
  {
    write(buffer[n]);
  }
  return size;

  /*
  size_t n = 0;
  while (size--) {
    n += write(*buffer++);
  }
  return n;
  */
}

size_t Print::print(const __FlashStringHelper *ifsh)
{
  PGM_P p = reinterpret_cast<PGM_P>(ifsh);
  size_t n = 0;

  // This way saves us 4 bytes, again we assume that
  // write of a single byte always returns 1 (as it does
  // for all the Serial classes in this core).
  //
  // J.Sleeman (sparks@gogo.co.nz)

  do 
  {
    unsigned char c = pgm_read_byte(p++);
    if (c == 0) break;
    write(c);
  } while(++n);

  /*
  while (1) {
    unsigned char c = pgm_read_byte(p++);
    if (c == 0) break;
    if (write(c)) n++;
    else break;
  }
  */

  return n;
}

size_t Print::print(const String &s)
{
  size_t n = 0;
  for (uint16_t i = 0; i < s.length(); i++) 
  {
    n += write(s[i]);
  }
  return n;
}

size_t Print::print(const char str[])
{
  return write(str);
}

// print of a char must always just pass straight to write
// in order to print the actual character rather than it's value
size_t Print::print(char c)
{
  return write(c);
}

#if PRINT_MAX_INT_TYPE != PRINT_INT_TYPE_BYTE
size_t Print::print(unsigned char b, uint8_t base)
{
  return print((UNSIGNED_PRINT_INT_TYPE) b, base);
}
#endif

#if PRINT_MAX_INT_TYPE != PRINT_INT_TYPE_INT
size_t Print::print(int n, uint8_t base)
{
  return print((PRINT_INT_TYPE) n, base);
}

size_t Print::print(unsigned int n, uint8_t base)
{
  return print((UNSIGNED_PRINT_INT_TYPE) n, base);
}
#endif

#if PRINT_MAX_INT_TYPE != PRINT_INT_TYPE_LONG
size_t Print::print(long n, uint8_t base)
{
  return print((PRINT_INT_TYPE) n, base);
}

size_t Print::print(unsigned long n, uint8_t base)
{
  return print((UNSIGNED_PRINT_INT_TYPE) n, base);
}
#endif

size_t Print::print(PRINT_INT_TYPE n, uint8_t base)
{
  /* Optimisation - not testing for invalid base
   * saves us some bytes, printNumber will "promote"
   * it to a valid base (2) anyway.
   *
   * The long-winded negative sign for base 10
   * is simplified to remove the temporary variable
   * sames us some more bytes
   *
   * I get 46 bytes less flash usage with mine.  On a tiny13
   * that's MASSIVE!
   *
   * J.Sleeman (sparks@gogo.co.nz)
   */

  /*

  if (base == 0)
  {
    return write(n);
  } 
  else if (base == 10)
  {
    int t = 0;
    if (n < 0)
    {
      t = print('-');
      n = -n;
    }
    return printNumber(n, 10) + t;
  }
  */

  if (base == 10 && n < 0)
  {
    // Interesting, this single liner makes it 16 bytes larger
    // than the 2 lines.
    //    return write('-') + printNumber(-n,base);
    write('-');
    return printNumber(-n,base)+1;
  }
  else
  {
    return printNumber(n, base);
  }
}

size_t Print::print(UNSIGNED_PRINT_INT_TYPE n, uint8_t base)
{
  /* As above, invalid base gets promoted in printNumber()
   * no need to look for it here.

  if (base == 0) return write(n);
  else return printNumber(n, base);
  */
  return printNumber(n,base);
}

size_t Print::print(double n, uint8_t digits)
{
  return printFloat(n, digits);
}

size_t Print::println(const __FlashStringHelper *ifsh)
{
  size_t n = print(ifsh);
  n += println();
  return n;
}

size_t Print::println(void)
{
  // I'm gonna assume that printing a single character
  // is only every going to return 1, so we don't need
  // to keep track of this, saves us 10 bytes of flash
  //
  // Note that this is true for Hardware,TinySoftware
  // and Basic Serial classes, they all blindly return
  // 1 as the result of a ::write(byte) regardless if
  // there is any notion of "success" or not.
  //
  // J.Sleeman (sparks@gogo.co.nz)
  //

  /*
  size_t n = print('\r');
  n += print('\n');
  return n;
  */

  print('\r');
  print('\n');
  return 2;
}

size_t Print::println(const String &s)
{
  size_t n = print(s);
  n += println();
  return n;
}

size_t Print::println(const char c[])
{
  size_t n = print(c);
  n += println();
  return n;
}

size_t Print::println(char c)
{
  size_t n = print(c);
  n += println();
  return n;
}
#if PRINT_MAX_INT_TYPE != PRINT_INT_TYPE_BYTE
size_t Print::println(unsigned char b, uint8_t base)
{
  size_t n = print(b, base);
  n += println();
  return n;
}
#endif
#if PRINT_MAX_INT_TYPE != PRINT_INT_TYPE_INT
size_t Print::println(int num, uint8_t base)
{
  size_t n = print(num, base);
  n += println();
  return n;
}

size_t Print::println(unsigned int num, uint8_t base)
{
  size_t n = print(num, base);
  n += println();
  return n;
}
#endif
#if PRINT_MAX_INT_TYPE != PRINT_INT_TYPE_LONG
size_t Print::println(long num, uint8_t base)
{
  size_t n = print(num, base);
  n += println();
  return n;
}

size_t Print::println(unsigned long num, uint8_t base)
{
  size_t n = print(num, base);
  n += println();
  return n;
}
#endif
size_t Print::println(PRINT_INT_TYPE num, uint8_t base)
{
  size_t n = print(num, base);
  n += println();
  return n;
}

size_t Print::println(UNSIGNED_PRINT_INT_TYPE num, uint8_t base)
{
  size_t n = print(num, base);
  n += println();
  return n;
}

size_t Print::println(double num, int digits)
{
  size_t n = print(num, digits);
  n += println();
  return n;
}

// Private Methods /////////////////////////////////////////////////////////////


#ifndef PRINT_USE_BASE_ARBITRARY

// This is a more memory (RAM and FLASH) efficient printNumber implementation
// for very low ram machines, eg tiny13, the original was way too big
// for several reasons...
//
// 1. It uses unsigned long
// 2. It uses division (an expensive operation on attiny)
// 3. It uses a character buffer of effectively 33 bytes
//    in order to build the number before writing it out
//
// When you only have 64 bytes of ram in the first place, that doesn't end well.
//
// This produces smaller flash and sram, even though it looks bigger with all these lookup tables
// the reduction in the number of assembly instructions more than makes up for it.
//
// Credit goes to EEVBlog User "Kalvin"
//  http://www.eevblog.com/forum/microcontrollers/memory-efficient-int-to-chars-without-division-(bitshift-ok)-for-binary-bases/msg805172/#msg805172
//
// The only downside is that it can't do arbitrary bases, just 2, 8, 16 and 10
//
// In your pins_arduino.h you will want to set
//
//  #define PRINT_USE_BASE_BIN
//  #define PRINT_USE_BASE_HEX
//  #define PRINT_USE_BASE_OCT
//  #define PRINT_USE_BASE_DEC
//
// if you define PRINT_USE_BASE_ARBITRARY then this function will not be used
// it will suck your ram, but you can use arbitrary bases.


size_t Print::printNumber(UNSIGNED_PRINT_INT_TYPE n, uint8_t base)
{
  static const char digits[] PROGMEM = "0123456789ABCDEF";

  #if defined(PRINT_USE_BASE_BIN)
  static const UNSIGNED_PRINT_INT_TYPE base2[]  PROGMEM =
  {
    #if PRINT_MAX_INT_TYPE == PRINT_INT_TYPE_LONG
        0x80000000, 0x40000000, 0x20000000, 0x10000000,  0x800000, 0x400000, 0x200000, 0x100000, 0x80000, 0x40000, 0x20000, 0x10000,
    #endif
    #if (PRINT_MAX_INT_TYPE == PRINT_INT_TYPE_LONG) || (PRINT_MAX_INT_TYPE == PRINT_INT_TYPE_INT)
      0x8000, 0x4000, 0x2000, 0x1000, 0x800, 0x400, 0x200, 0x100,
    #endif
      0x80, 0x40, 0x20, 0x10, 0x8, 0x4, 0x2, 0x1, 0
  };
  #endif
  #if defined(PRINT_USE_BASE_OCT)
  static const UNSIGNED_PRINT_INT_TYPE base8[]  PROGMEM =
  {
    #if PRINT_MAX_INT_TYPE == PRINT_INT_TYPE_LONG
      010000000000, 01000000000, 0100000000, 010000000, 01000000,
    #endif
    #if (PRINT_MAX_INT_TYPE == PRINT_INT_TYPE_LONG) || (PRINT_MAX_INT_TYPE == PRINT_INT_TYPE_INT)
      0100000, 010000,  01000,
    #endif
      0100, 010, 01,
      0
  };
  #endif
  #if defined(PRINT_USE_BASE_DEC)
  static const UNSIGNED_PRINT_INT_TYPE base10[] PROGMEM =
  {
    #if PRINT_MAX_INT_TYPE == PRINT_INT_TYPE_LONG
      1000000000,100000000,10000000,1000000,100000,
    #endif
    #if (PRINT_MAX_INT_TYPE == PRINT_INT_TYPE_LONG) || (PRINT_MAX_INT_TYPE == PRINT_INT_TYPE_INT)
      10000,  1000,
    #endif
      100,    10,  1,
      0
  };
  #endif
  #if defined(PRINT_USE_BASE_HEX)
  static const UNSIGNED_PRINT_INT_TYPE base16[] PROGMEM =
  {
    #if PRINT_MAX_INT_TYPE == PRINT_INT_TYPE_LONG
      0x10000000, 0x1000000,
        0x100000,  0x010000,
    #endif
    #if (PRINT_MAX_INT_TYPE == PRINT_INT_TYPE_LONG) || (PRINT_MAX_INT_TYPE == PRINT_INT_TYPE_INT)
          0x1000,    0x0100,
    #endif
            0x10,      0x01,
                          0
  };
  #endif

  UNSIGNED_PRINT_INT_TYPE const * bt;
  uint8_t leadingzero = 0;
  switch(base)
  {
    default:
      #if defined(PRINT_USE_BASE_HEX)
        write('x');
        base = 16;
      #elif defined(PRINT_USE_BASE_DEC)
        write('d');
        base = 10;
      #elif defined(PRINT_USE_BASE_OCT)
        write('o');
        base = 8;
      #elif defined(PRINT_USE_BASE_BIN)
        write('b');
        base = 2;
      #endif

    #ifdef PRINT_USE_BASE_HEX
      // Fall through
      case 16: bt = base16; break;
    #endif
    #ifdef PRINT_USE_BASE_DEC
      // Fall through
      case 10: bt = base10; break;
    #endif
    #ifdef PRINT_USE_BASE_OCT
      // Fall through
      case 8:  bt = base8;  break;
    #endif
    #ifdef PRINT_USE_BASE_BIN
      // Fall through
      case 2:  bt = base2;  break;
    #endif
  }

  // Reuse base for counting the digits since we don't need it any more.
  base = 0;
  do
  {
    UNSIGNED_PRINT_INT_TYPE b = PGM_READ_MAX_INT_TYPE(&*bt++);
    uint8_t digit = 0;
    while (n >= b)
    {
      digit++;
      n = n - b;
    }
    leadingzero = leadingzero ? leadingzero : digit;
    if (b == 1 || leadingzero)
    {
      ++base;
      write(pgm_read_byte(&digits[digit]));
    }
  }
  while (PGM_READ_MAX_INT_TYPE(&*bt));
  return base;
}


#else

// This is the original printNumber from Arduino with just the small change to
// allow you to set the integer type in use (see Print.h for how to set
// PRINT_MAX_INT_TYPE for your requirements in pins_arduino.h).
//
// This function allows arbitrary bases, but is very VERY heavy especially
// if you are not using division anywhere else in your program.
//
// VERY, VERY HEAVY

size_t Print::printNumber(UNSIGNED_PRINT_INT_TYPE n, uint8_t base)
{

  // This is super wasteful because it assumes you are using binary in the
  // worst case and makes a buffer big enough to show it in binary
  // which is 1 byte per bit
  char buf[8 * sizeof(n) + 1]; // Assumes 8-bit chars plus zero byte.

  char *str = &buf[sizeof(buf) - 1];

  *str = '\0';

  // prevent crash if called with base == 1
  if (base < 2) base = 10;

  do
  {
    UNSIGNED_PRINT_INT_TYPE m = n;
    n /= base;
    char c = m - base * n;
    *--str = c < 10 ? c + '0' : c + 'A' - 10;
  } while(n);

  return write(str);
}
#endif

size_t Print::printFloat(double number, uint8_t digits)
{
  size_t n = 0;

  // Handle negative numbers
  if (number < 0.0)
  {
     n += print('-');
     number = -number;
  }

  // Round correctly so that print(1.999, 2) prints as "2.00"
  double rounding = 0.5;
  for (uint8_t i=0; i<digits; ++i)
  {
    rounding /= 10.0;
  }

  number += rounding;

  // Extract the integer part of the number and print it
  unsigned long int_part = (unsigned long)number;
  double remainder = number - (double)int_part;
  n += print(int_part);

  // Print the decimal point, but only if there are digits beyond
  if (digits > 0)
  {
    n += print('.');
  }

  // Extract digits from the remainder one at a time
  while (digits-- > 0)
  {
    remainder *= 10.0;
    int toPrint = int(remainder);
    n += print(toPrint);
    remainder -= toPrint;
  }

  return n;
}
