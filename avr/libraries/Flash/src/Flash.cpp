#include "Flash.h"

/**
 * @brief Construct a new Flash::Flash object
 *
 */
Flash::Flash()
{
}

/**
 * @brief Erases a flash page
 *
 * @param page_number Page number in the allocated flash space to erase
 */
void Flash::erase_page(uint8_t page_number)
{
  uint8_t old_sreg = SREG;
  cli();
  eeprom_busy_wait();
  boot_page_erase(flash_space + page_number * SPM_PAGESIZE);
  SREG = old_sreg;
  flash_index = 0;
}

/**
 * @brief Write data to the internal flash buffer. This is data that will
 * eventually be stored in flash
 *
 * @param page_number Page number to fill
 * @param data 8-bit data to write
 * @return true if flash page is full
 * @return false if flash page is not full
 */
bool Flash::fill_page(uint8_t page_number, uint8_t data)
{
  static uint8_t low_byte;

  if(flash_index >= SPM_PAGESIZE)
  {
    flash_index = SPM_PAGESIZE;
    return true;
  }

  if((flash_index & 0x01)) // We must write words
    boot_page_fill(flash_space + page_number * SPM_PAGESIZE + flash_index, (data << 8) | low_byte);
  else
    low_byte = data;

  flash_index++;

  return false;
}

/**
 * @brief Writes the data stored in the flash buffer to the flash memory
 *
 * @param page_number Page number to write
 */
void Flash::write_page(uint8_t page_number)
{
  // Write one last time if we have an odd number of bytes in the buffer
  if(!(flash_index & 0x01))
    fill_page(page_number, 0x00);

  uint8_t old_sreg = SREG;
  cli();
  boot_page_write(flash_space + page_number * SPM_PAGESIZE);
  boot_spm_busy_wait();
  SREG = old_sreg;
}

/**
 * @brief Reads a byte from the flash memory
 *
 * @param page_number Page number to read
 * @param address Address in flash page to read
 * @return uint8_t 8-bit value
 */
uint8_t Flash::read(uint8_t page_number, uint8_t address)
{
  return pgm_read_byte(flash_space + page_number * SPM_PAGESIZE + address);
}

/**
 * @brief Returns the actual flash memory address for a particular byte in a page
 *
 * @param page_number
 * @param address
 * @return uint16_t
 */
uint16_t Flash::get_address(uint8_t page_number, uint8_t address)
{
  return (uint16_t)&flash_space + page_number * SPM_PAGESIZE + address;
}

uint8_t Flash::flash_index;
