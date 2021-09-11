/***********************************************************************|
| Flash read/write library for ATtiny13                                 |
|                                                                       |
| Flash_iterate.ino                                                     |
|                                                                       |
| A light-weight library that lets you read and write to the internal   |
| flash memory without using a RAM buffer.                              |
| Developed by MCUdude, based on the AVR Libc boot.h                    |
| https://github.com/MCUdude/MicroCore                                  |
|                                                                       |
| In this example we fill a flash page with 16-bit and read it back     |
| togeher with its physical flash address after a reboot.               |
|                                                                       |
| SERIAL REMINDER                                                       |
| The baud rate is ignored on the ATtiny13 due to using a simplified    |
| serial. The baud rate used is dependent on the processor speed.       |
| Note that you can specify a custom baud rate if the following ones    |
| does not fit your application.                                        |
|                                                                       |
| THESE CLOCKS USES 115200 BAUD:       THIS CLOCK USES 57600 BAUD:      |
| 20 MHz, 16 MHz, 9.6 MHz, 8 MHz       4.8 MHz                          |
| THESE CLOCKS USES 19200 BAUD:        THIS CLOCK USES 9600 BAUD:       |
| 1.2 MHz, 1 MHz                       600 KHz                          |
|                                                                       |
| If you get garbage output:                                            |
|  1. Check baud rate as above                                          |
|  2. Check if you have anything else connected to TX/RX like an LED    |
|  3. Check OSCCAL (see MicroCore OSCCAL tuner example)                 |
|***********************************************************************/

#include <Flash.h>

// Number of flash pages to allocated for read/write
#define NUMBER_OF_PAGES  1

// Allocated flash space
const uint8_t flashSpace[SPM_PAGESIZE * NUMBER_OF_PAGES] __attribute__((aligned(SPM_PAGESIZE))) PROGMEM = {};

// Pass allocated space pointer to Flash library
const uint8_t *Flash::flash_space = flashSpace;

void write_data()
{
  Serial.print(F("Writing...\n"));

  // Erase page 0
  flash.erase_page(0);

  // Fill the page buffer with the alfabet
  for(uint16_t i = 0xF000; i < 0xF100; i+= 0x10)
  {
    flash.put(0, i);

    // Alternatively, fill_page can be used instead
    //flash.fill_page(0, (uint8_t)i & 0xFF);
    //flash.fill_page(0, (uint8_t)(i >> 8));
  }

  // Write the page buffer to flash
  flash.write_page(0);

  // Reset the microcontroller to check for new content in flash
  Serial.print(F("Done! Please reset..."));
}

void read_data()
{
  Serial.print(F("Reading...\n"));


  for(uint8_t i = 0; i < SPM_PAGESIZE; i+=2)
  {
    uint16_t data;
    flash.get(0, i, data);
    Serial.print(F("Flash addr: 0x"));
    Serial.print(flash.get_address(0, i), HEX);
    Serial.print(F(", data = 0x"));
    Serial.print(data, HEX);
    Serial.print(F("\n"));
  }
}

void setup()
{
  Serial.begin();

  // Check for content by reading the first byte on page 0
  uint16_t data_present;
  flash.get(0, 0, data_present);

  // No data present - write new data
  if(data_present != 0xF000)
    write_data();

  // Data present - read from flash
  else
    read_data();
}

void loop()
{

}
