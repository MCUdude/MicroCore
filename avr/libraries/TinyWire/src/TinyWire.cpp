#include "TinyWire.h"

TinyWire Wire;

TinyWire::TinyWire()
{

}

void TinyWire::begin()
{
  // Todo: implement slave mode
}

void TinyWire::beginTransmission(uint8_t addr)
{
  start();
  status = write(addr << 1);   // Address goes in upper 7 bits
}

uint8_t TinyWire::write(uint8_t data)
{
  rw(data);
  scl_hi();

  uint8_t err;
  // Optimization trick saves 4 bytes w/ avr-gcc
  asm volatile ("clr %0" : "=r" (err));
  // NACK = SDA high
  if (I2C_PIN & (1 << SDA))
    err = ADDR_NAK;
  I2C_DDR |= (1 << SCL);          // SCL low

  return err;
}

uint8_t TinyWire::requestFrom(uint8_t addr)
{
  start();
  return write(addr << 1 | 1);   // Address goes in upper 7 bits
}

uint8_t TinyWire::restartRead(uint8_t addr)
{
  scl_hi();
  return requestFrom(addr);
}

uint8_t TinyWire::endTransmission()
{
  I2C_DDR |= (1 << SDA);          // SDA low
  _delay_us(TWICLK(FREQ));
  I2C_DDR &= ~(1 << SCL);         // Release SCL
  asm ("lpm" ::: "r0");           // 3 cycle delay
  _delay_us(TWICLK(FREQ));
  I2C_DDR &= ~(1 << SDA);         // Release SDA
  return status;
}

// Ack !=0 to send ack
uint8_t TinyWire::read(uint8_t ack)
{
  uint8_t data = rw(0xFF);

  if (ack)
    I2C_DDR |= (1 << SDA);      // SDA low = ACK
  scl_hi();
  asm ("nop");                  // Delay
  I2C_DDR |= (1 << SCL);        // SCL low

  return data;
}

uint8_t TinyWire::readAck()
{
  return read(1);
}

uint8_t TinyWire::readNack()
{
  return read(0);
}

void TinyWire::start()
{
  // Set SDA to output mode for low
  I2C_DDR |= (1 << SDA);
  asm ("lpm" ::: "r0");      // 3 cycle delay
  // Set SCL to output mode for low
  I2C_DDR |= (1 << SCL);
}

__attribute__((always_inline)) inline void TinyWire::scl_hi()
{
  _delay_us(TWICLK(FREQ));
  // Set SCL to input mode for high
  I2C_DDR &= ~(1 << SCL);
  // Check for clock stretching
  while ((I2C_PIN & (1 << SCL)) == 0);
  _delay_us(TWICLK(FREQ));
}

uint8_t TinyWire::rw(uint8_t data)
{
  uint8_t bits = 8;
  do
  {
    I2C_DDR &= ~(1 << SDA);     // SDA high
    if (!(data & 0x80))
      I2C_DDR |= (1 << SDA);
    scl_hi();
    uint8_t i2c_pin = I2C_PIN;
    I2C_DDR |= (1 << SCL);      // SCL low
    data <<= 1;
    if (i2c_pin & (1 << SDA))
      data |= 0x01;
  }
  while (--bits);

  I2C_DDR &= ~(1 << SDA);      // Release SDA
  return data;
}

uint8_t TinyWire::status;
