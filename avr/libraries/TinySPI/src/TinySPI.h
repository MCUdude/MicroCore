/*
 * Software based SPI Master Library for ATtiny13.
 *
 * Copyright (c) 2012 by Thomas Carpenter
 *               2017 by James Sleeman
 *               2017 by MCUdude
 *
 *
 * Modified 2017 by MCUdude
 *   - Rename library to TinySPI 
 *   - Replace digitalRead and digitalWrite PORTB and PINB
 *   - Get rid of "unused parameter" warnings by adding the unused attribute
 *   - Add examples
 *
 * Modified 2017 by James Sleeman
 *   - Use only the "Hard Coded" (in pins_arduino.h for each variant) MOSI/MISO/SCK
 *   - Do not do anything with SS, leave this up to the user, this is Master only anyway
 *   - Optimise (for space not cycles) byte reversal with assembly
 *   - Remove direct port manipulation as when used with my ATTinyCore fork 
 *       these optimisations are done by the normal functions when given compile-time-constants
 *       (as they will be now since using hard coded MOSI/MISO/SCK)
 *   - Remove "noTransfer", default to transferMode0 to start with, and do not 
 *       change transfer mode on end()
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 *
 */
 
#ifndef _SPI_H_INCLUDED
// Uses the same guard as the SPI class as the two cannot be used together
#define _SPI_H_INCLUDED

#include <stdio.h>
#include <Arduino.h>

#define SPI_MODE0 0
#define SPI_MODE1 1
#define SPI_MODE2 2
#define SPI_MODE3 3

#define SPI_CLOCK_DIV4 0x00
#define SPI_CLOCK_DIV16 0x01
#define SPI_CLOCK_DIV64 0x02
#define SPI_CLOCK_DIV128 0x03
#define SPI_CLOCK_DIV2 0x04
#define SPI_CLOCK_DIV8 0x05
#define SPI_CLOCK_DIV32 0x06

class SPISettings 
{
  public:
    SPISettings(uint32_t clock, uint8_t bitOrder, uint8_t dataMode) {
      init_AlwaysInline(clock, bitOrder, dataMode);
    }
    
    SPISettings() {
      init_AlwaysInline(4000000UL, MSBFIRST, SPI_MODE0); 
    }
  
  private:
    void init_AlwaysInline(__attribute__((unused)) uint32_t clock, uint8_t bitOrder, uint8_t dataMode)
      __attribute__((__always_inline__)) {
      _bitOrder = bitOrder;
      _dataMode = dataMode;
    }
    
    uint8_t _bitOrder;
    uint8_t _dataMode;
    
    // Clock frequency is fixed currently, see setClockDivider
    // and also see the definition of SPISettings in the SPI Library itself
    // for implementation of determining a clock divider
    // uint8_t clockDiv = 64;
    
    friend class TinySoftSPIClass;
};

class TinySoftSPIClass;
typedef byte (TinySoftSPIClass::*TransferFunction)(byte _data);

class TinySoftSPIClass 
{
  public:
    TinySoftSPIClass();
    
  private:
    TransferFunction transferType;
    
    byte transferMode0(byte _data);
    byte transferMode1(byte _data);
    byte transferMode2(byte _data);
    byte transferMode3(byte _data);
    
  public:
    void begin();
    byte transfer(byte _data);
    void end();
    
    void setBitOrder(uint8_t);
    void setDataMode(uint8_t);
    void setClockDivider(uint8_t);
    
    void beginTransaction(SPISettings);
    void endTransaction();
    
    // Not Implemented
    // usingInterrupt
    
  private:	
    byte _bitOrder;
};

extern TinySoftSPIClass SPI;
#endif