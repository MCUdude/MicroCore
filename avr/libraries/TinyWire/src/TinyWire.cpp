#include "TinyWire.h"


/**
 * @brief Construct a new TinyWire object
 *
 */
TinyWire::TinyWire()
{

}

void TinyWire::begin()
{
  // Todo: implement slave mode
}

/**
 * @brief Starts an i2c transmission
 *
 * @param addr 7-bit slave i2c address
 * @return uint8_t Transmission status. 0 = success, 2 = NACK
 */
uint8_t TinyWire::beginTransmission(uint8_t addr)
{
  start();
  twi_status = write(addr << 1); // Address goes in upper 7 bits
  return twi_status;
}

/**
 * @brief Writes a byte to an i2c slave
 *
 * @param data Data to write
 * @return uint8_t Write status. 0 = success, 2 = NACK
 */
uint8_t TinyWire::write(uint8_t data)
{
  rw(data);
  scl_hi();

  uint8_t err = SUCCESS;
  asm volatile ("clr %0" : "=r" (err)); // Optimization trick saves 4 bytes w/ avr-gcc
  if (I2C_PIN & (1 << SDA))             // NACK = SDA high
    err = ADDR_NAK;
  I2C_DDR |= (1 << SCL);                // SCL low

  return err;
}

/**
 * @brief Request to read N bytes from i2c slave
 *
 * @param addr 7-bit slave i2c address
 * @param number_of_bytes number of bytes to read from the i2c slave
 * @return uint8_t Write status. 0 = success, 2 = NACK
 */
uint8_t TinyWire::requestFrom(uint8_t addr, uint16_t number_of_bytes)
{
  start();
  read_bytes = number_of_bytes;
  return write(addr << 1 | 1); // Address goes in upper 7 bits
}

/**
 * @brief Restarts a request to read N bytes from i2c slave
 *
 * @param addr 7-bit slave i2c address
 * @param number_of_bytes number of bytes to read from the i2c slave
 * @return uint8_t Write status. 0 = success, 2 = NACK
 */
uint8_t TinyWire::restartRead(uint8_t addr, uint16_t number_of_bytes)
{
  scl_hi();
  return requestFrom(addr, number_of_bytes);
}

/**
 * @brief End i2c transmission. Has to be executed after a read sequence too.
 *
 * @return uint8_t Transmission status. 0 = success, 2 = NACK
 */
uint8_t TinyWire::endTransmission()
{
  I2C_DDR |= (1 << SDA);  // SDA low
  _delay_us(TWICLK(FREQ));
  I2C_DDR &= ~(1 << SCL); // Release SCL
  _delay_us(TWICLK(FREQ));
  I2C_DDR &= ~(1 << SDA); // Release SDA
  return twi_status;
}

/**
 * @brief Read a byte from an i2c slave device where ACK/NACK is handled automatically
 *
 * @return uint8_t Data read from the i2c slave
 */
uint8_t TinyWire::read()
{
  if (read_bytes-- > 1) // Send ACK of we still have data to read
    return read(1);
  else
    return read(0);     // Send NACK if we're reading last byte
}

/**
 * @brief Read a byte from an i2c slave device where ACK/NACK is handled manually
 *
 * @param ack ACK flag. Pass 1 for ACK, and 0 for NACK
 * @return uint8_t Data read from the i2c slave
 */
uint8_t TinyWire::read(uint8_t ack)
{
  uint8_t data = rw(0xFF);

  if (ack)
    I2C_DDR |= (1 << SDA); // SDA low = ACK
  scl_hi();
  I2C_DDR |= (1 << SCL);   // SCL low

  return data;
}

/**
 * @brief Low level i2c start routine
 *
 */
void TinyWire::start()
{
  I2C_DDR |= (1 << SDA); // Set SDA to output mode for low
  asm ("lpm" ::: "r0");  // 3 cycle delay
  I2C_DDR |= (1 << SCL); // Set SCL to output mode for low
}

/**
 * @brief Low level set clock high routine
 *
 */
__attribute__((always_inline)) inline void TinyWire::scl_hi()
{
  _delay_us(TWICLK(FREQ));
  I2C_DDR &= ~(1 << SCL);              // Set SCL to input mode for high
  while ((I2C_PIN & (1 << SCL)) == 0); // Check for clock stretching
  _delay_us(TWICLK(FREQ));
}

/**
 * @brief Low level read/write routine
 *
 * @param data Data to send. Pass 0xFF to read
 * @return uint8_t Data read from the i2c slave
 */
uint8_t TinyWire::rw(uint8_t data)
{
  uint8_t bits = 8;
  do
  {
    I2C_DDR &= ~(1 << SDA); // SDA high
    if (!(data & 0x80))
      I2C_DDR |= (1 << SDA);
    scl_hi();
    uint8_t i2c_pin = I2C_PIN;
    I2C_DDR |= (1 << SCL); // SCL low
    data <<= 1;
    if (i2c_pin & (1 << SDA))
      data |= 0x01;
  }
  while (--bits);

  I2C_DDR &= ~(1 << SDA); // Release SDA
  return data;
}

/**
 * @brief Holds the i2c transmission status shared between
 * startTransmission() and endTransmission()
 */
uint8_t  TinyWire::twi_status;

/**
 * @brief Holds the number of bytes to read from a slave.
 * This variable is set in requestFrom() and used to keep track
 * of ACK/NACK in read().
 *
 */
uint16_t TinyWire::read_bytes;
