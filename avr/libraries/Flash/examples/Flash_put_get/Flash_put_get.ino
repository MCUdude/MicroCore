/***********************************************************************|
| Flash read/write library for ATtiny13                                 |
|                                                                       |
| Flash_put_get.ino                                                     |
|                                                                       |
| A light-weight library that lets you read and write to the internal   |
| flash memory without using a RAM buffer.                              |
| Developed by MCUdude, based on the AVR Libc boot.h                    |
| https://github.com/MCUdude/MicroCore                                  |
|                                                                       |
| In this example we demonstrate how variables and strings can be       |
| read from and written to the flash memory.                            |
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
#define NUMBER_OF_PAGES  2

// Allocated flash space
const uint8_t flashSpace[SPM_PAGESIZE * NUMBER_OF_PAGES] __attribute__((aligned(SPM_PAGESIZE))) PROGMEM = {};

// Pass allocated space pointer to Flash library
const uint8_t *Flash::flash_space = flashSpace;

void write_data()
{
  Serial.print(F("Writing...\n"));

  // Erase page 0
  flash.erase_page(0);

  // Write unsigned 16-bit integer value
  uint16_t flash_content = 100;
  flash.put(0, flash_content);

  // Write bool value
  bool flag = true;
  flash.put(0, flag);

  // Write string
  const char text[] = {"Hello World!"};
  flash.put(0, text);

  // Write data to page 0
  flash.write_page(0);

  // Erase page 1 so it can be written to
  flash.erase_page(1);

  // Fill the page buffer with the alfabet
  for(char i = 'A'; i <= 'Z'; i++)
    flash.put(1, i);

  // Write the page buffer to flash
  flash.write_page(1);

  // Reset the microcontroller to check for new content in flash
  Serial.print(F("Done! Please reset..."));
}

void read_data()
{
  Serial.print(F("Reading...\n"));

  // Empty variables to store content to
  uint16_t i;
  bool flag;
  char text[12];
  uint8_t buffer_address = 0; // Buffer address to start from

  // Read integer value from flash page 0
  flash.get(0, buffer_address, i);
  Serial.print(F("i = "));
  Serial.print(i);

  // Read bool value from flash page 0
  buffer_address += sizeof(int); // Move address to the next byte after float 'f'
  flash.get(0, buffer_address, flag);
  Serial.print(F("\nflag = "));
  Serial.print(flag);
  Serial.print('\n');

  // Read text string from lash page 0
  buffer_address += sizeof(bool);
  flash.get(0, buffer_address, text);
  Serial.print(text);
  Serial.print('\n');

  // Read text string from flash page 1 using flash.read()
  for(uint8_t i = 0; i < SPM_PAGESIZE; i++)
  {
    char c = flash.read(1, i);
    Serial.print(c);
    Serial.print(' ');
  }
}

void setup()
{
  Serial.begin();

  // Check for content by reading the first byte on page 0
  uint16_t data_present = flash.read(0, 0);

  // No data present - write new data
  if(data_present != 100)
    write_data();

  // Data present - read from flash
  else
    read_data();
}

void loop()
{

}
