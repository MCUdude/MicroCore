
#pragma once

#include <stdint.h>
#include <avr/io.h>

#ifndef I2C_DDR
  #define I2C_DDR DDRB
  #define I2C_PIN PINB
#endif

const uint8_t SUCCESS = 0;
const uint8_t ADDR_NAK = 2;

class TinyWire
{
  public:
    TinyWire();
    static void begin();
    static void beginTransmission(uint8_t addr);
    static uint8_t write(uint8_t data);       // Start must be called first
    static uint8_t requestFrom(uint8_t addr);
    static uint8_t restartRead(uint8_t addr);
    static uint8_t endTransmission();
    static uint8_t read(uint8_t ack = 1);   // ack !=0 to send ack
    static uint8_t readAck();
    static uint8_t readNack();

  private:
    static void start();
    __attribute__((always_inline)) static inline void scl_hi();
    // Combined i2c read/write routine. Pass 0xFF when reading
    static uint8_t rw(uint8_t data);

    // PORT pin numbers for SDA & SCL (0-7)
    static const uint8_t SCL;
    static const uint8_t SDA;

    static uint8_t status;
};

extern TinyWire Wire;
