# Flash
A light-weight library that lets you read and write to the internal flash memory without using a RAM buffer.
Developed by [MCUdude](https://github.com/MCUdude/).

The flash library is a semi high level library that utilizes AVR Libc's [boot.h](https://www.nongnu.org/avr-libc/user-manual/group__avr__boot.html). It is page oriented, which means that you'll have to write an entire page to flash at once. The ATtiny13 's flash pages is 32 bytes large.

## flash_space
Static constant pointer that has to point to the allocated flash space in the user program.
This tells the library where the allocated flash space is.

### Usage
```c++
// Number of flash pages to allocated for read/write
#define NUMBER_OF_PAGES  2

// Allocated flash space
const uint8_t flashSpace[SPM_PAGESIZE * NUMBER_OF_PAGES] __attribute__((aligned(SPM_PAGESIZE))) PROGMEM = {};

// Pass allocated space pointer to Flash library
const uint8_t *Flash::flash_space = flashSpace;
```


## erase_page()
Static function that erases a flash page.

### Declaration
```c++
static void erase_page(uint8_t page_number);
```

### Returns
`void`

### Usage
```c++
// Erase allocated flash page 0
flash.erase_page(0);
```


## fill_page()
Static function that is used to fill the flash page buffer. This buffer "lives" within the flash controller itself, and does not accupy any RAM. this buffer can only be written to, and the function handles the address increments automatically. Note that the buffer doesn't have to be full to write it to flash.

### Declaration
```c++
static bool fill_page(uint8_t page_number, uint8_t data);
```

### Returns
`true` if the buffer is full (32 bytes written).</br>
`false` if buffer is not full.

### Usage
```c++
// Write 0x55 to the next byte in the buffer on page 0
flash.fill_page(0, 0x55);
```


## write_page()
Writes the flash buffer content to a flash page.

### Declaration
```c++
static void write_page(uint8_t page_number);
```

### Returns
`void`

### Usage
```c++
// Write to flash page 0
flash.write_page(0);
```


## read()
Reads a byte from flash.

### Declaration
```c++
static uint8_t read(uint8_t page_number, uint8_t address);
```

### Returns
`uint8_t` byte stored in flash.

### Usage
```c++
// Read a byte from flash page 0, address 1
uint8_t byte_from_flash = flash.read(0, 1);
```


## get_address()
Returns the physical flash memory address for a particular byte in flash

### Declaration
```c++
static uint16_t get_address(uint8_t page_number, uint8_t address);
```

### Returns
`uint16_t` 16-bit memory address.

### Usage
```c++
// Get the physical flash memory address from data located at page 0, address 1
uint16_t flash_memory_address = flash.get_address(0, 1);
```


## put()
Write any data type or object to flash. This template function is a wrapper around fill_page(), and handles address increments automatically.

### Declaration
```c++
template <typename T> static const uint8_t &put(uint8_t page_number, const T &t);
```

### Returns
`const uint8_t` number of bytes currently in the flash buffer.

### Usage
```c++
// Write a string starting flash page 0, and then write the number to the same page
char my_str[] = "Hello World!";
uint16_t number = 12345;

flash.put(0, my_str);
flash.put(0, data);
```


## get()
Read any data type or object from flash.

### Declaration
```c++
template <typename T> static T &get(uint8_t page_number, uint8_t page_address, T &t)
```

### Returns
`T` value stored to passed variable.

### Usage
```c++
// Read a string from flash page 0 starting from address 0, and read the number afterwards
char my_str[13];
uint16_t number;
flash.get(0, 0, my_str);
flash.get(0, sizeof(my_str), number);
```