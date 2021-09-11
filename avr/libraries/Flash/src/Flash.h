/**
 * @file Flash.h
 * @author MCUdude (https://github.com/MCUdude)
 * @brief A light-weight library that lets you read and write to the internal
          flash memory without using a RAM buffer.
 * @date 2021-09-11
 * @copyright Copyright (c) 2021
 *
 */

#ifndef FLASH_H
#define FLASH_H

#include <Arduino.h>
#include <avr/boot.h>

class Flash
{
  public:
    Flash();
    static void erase_page(uint8_t page_number);
    static bool fill_page(uint8_t page_number, uint8_t data);
    static void write_page(uint8_t page_number);
    static uint8_t read(uint8_t page_number, uint8_t address);
    static uint16_t get_address(uint8_t page_number, uint8_t address);

    /**
     * @brief Template function to 'put' objects in the flash buffer.
     * Adressing within a page is handled automatically.
     *
     * @param page_number Page number to write to
     * @param t Variable/struct/object etc. to write to flash
     * @return const uint8_t& number of bytes currently in the flash buffer
     */
    template <typename T> static const uint8_t &put(uint8_t page_number, const T &t)
    {
      const uint8_t *ptr = (const uint8_t*) &t;
      for(uint8_t count = 0; count < sizeof(T); count++)
        fill_page(page_number, *ptr++);
      return flash_index;
    }

    /**
     * @brief Template function to 'get' objects from the flash memory
     *
     * @param page_number Page number to read from
     * @param page_address Address in flash page to start reading from
     * @param t Variable/struct/object etc. to store read data to
     * @return T& Data stored in passed variable
     */
    template <typename T> static T &get(uint8_t page_number, uint8_t page_address, T &t)
    {
      uint8_t *ptr = (uint8_t*) &t;
      for(uint8_t count = 0; count < sizeof(T); count++)
        *ptr++ = read(page_number, page_address + count);
      return t;
    }

    static const uint8_t *flash_space; // Pointer to allocated flash space

  private:
    static uint8_t flash_index; // Keeps track of the address within a page when filling the buffer

};

// Static object to use in user program
static Flash flash;

#endif
