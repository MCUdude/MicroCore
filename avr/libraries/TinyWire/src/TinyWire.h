
#ifndef TINYWIRE_H
#define TINYWIRE_H

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

#ifndef I2C_DDR
  #define I2C_DDR DDRB
  #define I2C_PIN PINB
#endif

// Crude formula to calculate approximate TWI clock speed based on F_CPU
#define TWICLK(FREQ) (((1000000.0f / (FREQ / 0.5)) < (12.0 * (1000000.0f / F_CPU)) ? 0.0 : ((1000000.0f / (FREQ / 0.5)) - (12.0 * (1000000.0f / F_CPU)))))

// Use TinyWire::FREQ = MAX_SPEED to get the highest speed possible, which is always way less than 2 MHz
const uint32_t MAX_SPEED = 2000000L;

// Return values for beginTransmission and endTransmission
enum
{
  SUCCESS  = 0,
  ADDR_NAK = 2,
};

class TinyWire
{
  public:
    TinyWire();
    static void    begin();
    static uint8_t beginTransmission(uint8_t addr);
    static uint8_t write(uint8_t data);
    static uint8_t requestFrom(uint8_t addr, uint16_t number_of_bytes = 1);
    static uint8_t restartRead(uint8_t addr, uint16_t number_of_bytes = 1);
    static uint8_t endTransmission();
    static uint8_t read();
    static uint8_t read(uint8_t ack); // ack > 0 to send ACK

    static const uint8_t  SCL;  // Port number for SDA pin
    static const uint8_t  SDA;  // Port number for SCL pin
    static const uint32_t FREQ; // Clock frequency

  private:
    static void start();
    __attribute__((always_inline)) static inline void scl_hi();
    static uint8_t rw(uint8_t data); // Combined i2c read/write routine. Pass 0xFF when reading

    static uint8_t  twi_status;
    static uint16_t read_bytes;
};

static TinyWire Wire;

#endif
