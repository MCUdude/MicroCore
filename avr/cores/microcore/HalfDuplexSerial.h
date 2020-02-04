/* optimized half-duplex high-speed serial uart implementation
 * @author: Ralph Doncaster 2014
 * @version: $Id$
 * soft UART has only 0.8% timing error at default 115200 baud rate @8Mhz
 * and 2.1% timing error at 230400.
 *
 * define BAUD_RATE before including HalfDuplexSerial.h to change default baud rate
 *
 * Modified by J.Sleeman (sparks@gogo.co.nz) to add Non Blocking reads
 *   RxByteNBZeroReturn   (returns BYTE 0 for no data [or null byte read])
 *   RxByteNBNegOneReturn (returns INT -1 for no data)
 * if not used these should be optimized out automatically by the linker.
 *
 * Wrapped in Stream/Print class to be able to use as Serial by J.Sleeman
 *
 * Modified by MCUdude for the MicroCore
 * https://github.com/MCUdude/MicroCore
 */

#ifndef HalfDuplexSerial_h
#define HalfDuplexSerial_h

#include <inttypes.h>
#include <stdio.h> // for size_t
#include "Arduino.h"
#include "WString.h"
#include "core_settings.h"

#define DEC  ((uint8_t) 10)
#define HEX  ((uint8_t) 16)
#define OCT  ((uint8_t)  8)
#define BIN  ((uint8_t)  2)
#define BYTE ((uint8_t)  0)

// Maximum integer type to be handled.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// For small machines (tiny13 etc) handling long may be too demanding
// so we can now specify that int is the largest type we can handle
//
// In other words, put this in your pins_arduino.h file...
//    #define PRINT_MAX_INT_TYPE PRINT_INT_TYPE_INT
// (or pass in buld.extra_flags=-DPRINT_MAX_INT_TYPE=2)
//
// The possible options are exactly these:
//
// PRINT_INT_TYPE_LONG  (default)
// PRINT_INT_TYPE_INT
// PRINT_INT_TYPE_BYTE
//
//

#define PRINT_INT_TYPE_LONG 1
#define PRINT_INT_TYPE_INT  2
#define PRINT_INT_TYPE_BYTE 3

#ifndef PRINT_MAX_INT_TYPE
  // The official Arduino core can do long, so this is the default
  // if your variant doesn't specify otherwise.
  #define PRINT_MAX_INT_TYPE    PRINT_INT_TYPE_LONG
#endif

#if PRINT_MAX_INT_TYPE == PRINT_INT_TYPE_LONG
  #define PRINT_INT_TYPE            int32_t
  #define UNSIGNED_PRINT_INT_TYPE   uint32_t
  #define PGM_READ_MAX_INT_TYPE(A)  pgm_read_dword(A)
#elif PRINT_MAX_INT_TYPE == PRINT_INT_TYPE_INT
  #define PRINT_INT_TYPE            int16_t
  #define UNSIGNED_PRINT_INT_TYPE   uint16_t
  #define PGM_READ_MAX_INT_TYPE(A)  pgm_read_word(A)
#elif PRINT_MAX_INT_TYPE == PRINT_INT_TYPE_BYTE
  #define PRINT_INT_TYPE            int8_t
  #define UNSIGNED_PRINT_INT_TYPE   uint8_t
  #define PGM_READ_MAX_INT_TYPE(A)  pgm_read_byte(A)
#endif


// Number Base printing support
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Tight memory systems (flash and sram) might need to restrict the number
// bases that can be printed, typically it's unnecessary
// to allow arbitrary bases and doing so really eats your sram (and flash).
//
// Set one or more of the following defines (in pins_arduino.h usually):
//
//   #define PRINT_USE_BASE_BIN
//   #define PRINT_USE_BASE_HEX
//   #define PRINT_USE_BASE_OCT
//   #define PRINT_USE_BASE_DEC
//   #define PRINT_USE_BASE_ARBITRARY
//
// if PRINT_USE_BASE_ARBITRARY is set the other defines are ignored
// and the normal Arduino print functions are in play.
//
// if it is not set then an optimised printNumber() function is used
// without division.
//
// If nothing is set then we use the following as the default:
//
// #define PRINT_USE_BASE_BIN
// #define PRINT_USE_BASE_HEX
// #define PRINT_USE_BASE_OCT
// #define PRINT_USE_BASE_DEC
//
// This gives us almost the same functionality as the standard
// arduino print, because 99% of people are only ever going to need
// base 2/8/10 and 16 in their life (frankly, I would omit 8 as well
// except that ASCIITable example uses it).
//
// If you try and print an "unsupported" base it's not a problem
// simply it will "promote" your request to the highest supported
// base and prefix the printed number with a letter to signal
// (b = binary, o = octal, d = decimal, x = hex)
// which base was used instead of what you requested.
//
// If you really want to use non-standard bases, set PRINT_USE_BASE_ARBITRARY
// but be aware that it will bring in a very large amount of overhead
// (mainly ram usage, not good if you only have 64 bytes of RAM!)
//
// For what it's worth the flash-usage for having only ONE base
// enabled was measured on a sketch as follows...
//
// BIN      = lowest
// OCT      = 6 bytes more than BIN only
// HEX      = 6 bytes more than BIN only
// DEC      = highest - 22 bytes more than BIN
//
// Some combinations relative to BIN only:
// BIN+OCT         = +36 bytes
// BIN+HEX         = +52 bytes
// BIN+HEX+OCT     = +72 bytes
// BIN+DEC         = +94 bytes
// DEC+HEX         = +98 bytes
// BIN+HEX+DEC     = +122 bytes
// BIN+HEX+DEC+OCT = +150 bytes
//
// Short version, if you can live with only one supported base
// for printing numbers, you save a whole pile of memory no matter
// which base you choose, even DEC on it's own doesn't eat much
// more than BIN on it's own, but as soon as you start supporting
// more than one base, you start eating flash.
//
// Of course, if you don't actually use anything that calls
//    print(numberhere, [optionalbase])
// then it's all optimised away by gcc and you don't care.
//

#if ! (defined(PRINT_USE_BASE_BIN) || defined(PRINT_USE_BASE_HEX) || defined(PRINT_USE_BASE_OCT) || defined(PRINT_USE_BASE_DEC) || defined(PRINT_USE_BASE_ARBITRARY))
  #define PRINT_USE_BASE_BIN
  #define PRINT_USE_BASE_HEX
  #define PRINT_USE_BASE_OCT
  #define PRINT_USE_BASE_DEC
#endif

// Set default baud rate based on F_CPU
#ifndef CUSTOM_BAUD_RATE
  #if F_CPU >= 5000000L
    #define BAUD_RATE  115200
  #elif F_CPU >= 4800000L
    #define BAUD_RATE  57600  
  #elif F_CPU >= 1000000L
    #define BAUD_RATE  19200
  #elif F_CPU >= 600000L
    #define BAUD_RATE  9600
  #else
    #define BAUD_RATE  300
    #warning Clock speed too slow for serial communication!
  #endif
#else
  #define BAUD_RATE CUSTOM_BAUD_RATE
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void TxByte(unsigned char);
  unsigned char RxByte();

  // These two functions are non-blocking variants
  unsigned char RxByteNBZeroReturn();  // NBZeroReturn will return a zero when there is no byte read
  int16_t RxByteNBNegOneReturn();      // NBNegOneReturn returns -1; which is the same as standard Serial.write()

#ifdef __cplusplus
}
#endif

#define DIVIDE_ROUNDED(NUMERATOR, DIVISOR) ((((2*(NUMERATOR))/(DIVISOR))+1)/2)

// txbit takes 3*RXDELAY + 15 cycles
#define BIT_CYCLES DIVIDE_ROUNDED(F_CPU,BAUD_RATE*1L) 
#define TXDELAYCOUNT DIVIDE_ROUNDED(BIT_CYCLES - 7, 3)

#define RXSTART_CYCLES DIVIDE_ROUNDED(3*F_CPU,2L*BAUD_RATE) 
// 1st bit sampled 3*RXDELAY + 11 cycles after start bit begins
#define RXSTARTCOUNT DIVIDE_ROUNDED(RXSTART_CYCLES - 13, 3)
// rxbit takes 3*RXDELAY + 12 cycles
#define RXDELAYCOUNT DIVIDE_ROUNDED(BIT_CYCLES - 13, 3)

#if (RXSTARTCOUNT > 255)
  #if (F_CPU == 20000000L)
    #error Baud rate too low for a 20 MHz clock! Use 57600 baud or more
  #elif (F_CPU == 16000000L)
    #error Baud rate too low for a 16 MHz clock! Use 38400 baud or more
  #elif (F_CPU == 12000000L)
    #error Baud rate too low for a 12 MHz clock! Use 38400 baud or more
  #elif (F_CPU == 9600000L)
    #error Baud rate too low for a 9.6 MHz clock! Use 38400 baud or more
  #elif (F_CPU == 8000000L)
    #error Baud rate too low for a 16 MHz clock! Use 38400 baud or more
  #elif (F_CPU == 4800000L)
    #error Baud rate too low for a 4.8 MHz clock! Use 9600 baud or more
  #elif (F_CPU == 1200000L)
    #error Baud rate too low for a 1.2 MHz clock! Use 2400 baud or more
  #elif (F_CPU == 1000000L)
    #error Baud rate too low for a 1 MHz clock! Use 2400 baud or more
  #elif (F_CPU == 600000L)
    #error Baud rate too low for a 600kHz clock! Use 1200 baud or more
  #endif
#endif

asm(".global TXDELAY" );
asm(".global RXSTART" );
asm(".global RXDELAY" );

// dummy function defines no code
// hack to define absolute linker symbols using C macro calculations
static void dummy() __attribute__ ((naked));
static void dummy() __attribute__ ((used));
static void dummy()
{
asm (
    ".equ TXDELAY, %[txdcount]\n"
    ::[txdcount] "M" (TXDELAYCOUNT)
    );
asm (
    ".equ RXSTART, %[rxscount]\n"
    ::[rxscount] "M" (RXSTARTCOUNT)
    );
asm (
    ".equ RXDELAY, %[rxdcount]\n"
    ::[rxdcount] "M" (RXDELAYCOUNT)
    );
}

class HalfDuplexSerial
{
  private:
    void printNumber(UNSIGNED_PRINT_INT_TYPE, uint8_t);
    void printFloat(double, uint8_t);
  protected:
    void setWriteError(int err = 1) { (void)err; }
  public:
    void begin(const uint32_t) { } // Does NOTHING, you have no need to call this, here only for compatibility
    void begin() { }               // Does NOTHING, you have no need to call this, here only for compatibility
    void end() { }                 // Does NOTHING, you have no need to call this, here only for compatibility
    int16_t available();           // As we do not have a buffer, this always returns 0
    int16_t peek();                // As we do not have a buffer, this always returns -1
    void flush() { }               // Does NOTHING, you have no need to call this, here only for compatibility

    /** Read a byte, non-blocking.
     *
     * Caution, this is declared virtual in Stream::read() and it will not be optimized out even if not used.
     *
     * In order to get around this, we have the
     * HALF_DUPLEX_SERIAL_DISABLE_READ
     * define, if defined then read() is disabled by way of always returning -1 which will allow
     * the assembly code behind it to be optimized away
     *
     * @return int  -1 for no-data-read, 0 or greater is the byte read
     */

    int16_t read(void);

    /** Read a byte, non-blocking, will optimize-out.
     *
     * This is exactly the same as read() except that it's not part of Stream:: so it can be
     * optimized out if unused, as a result it is available for you to use even if you
     * define HALF_DUPLEX_SERIAL_DISABLE_READ
     *
     * @return int16_t  -1 for no-data-read, 0 or greater is the byte read
     */

    int16_t read_byte(void);

    /** Read a char, non-blocking.
     *
     * This is non blocking and returns 0 for no-data-read (OR if the data read was 0)
     * it is therefore convenient for reading a character into a character array
     * if(i>=sizeof(buf)-1 || !(buf[i++]=read_char())) break;
     * (you can insert a retry-timeout in the loop too for that matter)
     * remember if reading multiple characters, there is NO buffer, there is NO
     * handshaking, so your loop better be very tight or you will miss characters
     * or worse, read characters incorrectly by misinterpreting bits
     *
     * Optimized out if unused, always available.
     *
     * @return char (0 if nothing read or if the data read was 0)
     */

    char read_char(void);

    /** Read a char, blocking.
     *
     * Blocking read of a single char, doesn't return until it has a byte
     * so would be useful for a single character read where you only want
     * 1 character and can't continue until you have it (for example, a
     * menu selection).
     *
     * Optimized out if unused, always available.
     *
     * @return char (0 if the data read was 0)
     */

    char read_char_blocking(void);

    /** Read a variable length null terminated string.
     *
     * This is more-or-less non-blocking it will time-out after an
     * (indeterminate, very lazily counted) number of ms of activity.
     * The timeout is poorly implemented without the use of millis()
     * to keep code size and memory usage down.
     *
     * Because of null-termination the maximum string length is length-1
     * notice that we do not return the length of the string
     * it's null-terminated so you can easily work it out if you need to
     *
     * @param buf     The character buffer to put the read string into.
     * @param length  The length (sizeof()) the buffer.
     */

    void read_str(char buf[], byte length);

    /** Write a byte.
     *
     * Caution, this is declared virtual in Print::write() and it will not be optimized out even if not used.
     *
     * In order to get around this, we have the
     * HALF_DUPLEX_SERIAL_DISABLE_WRITE
     * define, if defined then write() is disabled by way of always returning -1 which will allow
     * the assembly code behind it to be optimized away.
     *
     * However you still can use write_byte() because that can be optimized out.
     *
     * @param ch Byte to write.
     * @return  Always returns 1
     */


    int16_t getWriteError() { return 0; }
    void clearWriteError() { setWriteError(0); }

    void write(uint8_t ch);
    void write(const uint8_t *buffer, size_t size);
    void write(const char *str) { return write((const uint8_t *)str, strlen(str)); }

    void print(const __FlashStringHelper *);
    void print(const String &);
    void print(const char[]);
    void print(char);

    // by using these conditionals we can cut-out some pointless
    // function calls and casting when our "primary" integer type
    // for print is the same as that particular function handles
    // otherwise these guarded functions will cast to the
    // primary type (PRINT_INT_TYPE or UNSIGNED_PRINT_INT_TYPE
    // as appropriate).
    #if PRINT_MAX_INT_TYPE != PRINT_INT_TYPE_BYTE
    void print(unsigned char, uint8_t = DEC);
    #endif
    #if PRINT_MAX_INT_TYPE != PRINT_INT_TYPE_INT
    void print(int, uint8_t = DEC);
    void print(unsigned int, uint8_t = DEC);
    #endif
    #if PRINT_MAX_INT_TYPE != PRINT_INT_TYPE_LONG
    void print(long, uint8_t = DEC);
    void print(unsigned long, uint8_t = DEC);
    #endif
    void print(PRINT_INT_TYPE, uint8_t = DEC);
    void print(UNSIGNED_PRINT_INT_TYPE, uint8_t = DEC);
    void print(double, uint8_t = 2);

    void println(const __FlashStringHelper *);
    void println(const String &s);
    void println(const char[]);
    void println(char);
    #if PRINT_MAX_INT_TYPE != PRINT_INT_TYPE_BYTE
    void println(unsigned char, uint8_t = DEC);
    #endif
    #if PRINT_MAX_INT_TYPE != PRINT_INT_TYPE_INT
    void println(int, uint8_t = DEC);
    void println(unsigned int, uint8_t = DEC);
    #endif
    #if PRINT_MAX_INT_TYPE != PRINT_INT_TYPE_LONG
    void println(long, uint8_t = DEC);
    void println(unsigned long, uint8_t = DEC);
    #endif
    void println(PRINT_INT_TYPE, uint8_t = DEC);
    void println(UNSIGNED_PRINT_INT_TYPE, uint8_t = DEC);
    void println(double, int = 2);
    void println(void);

};

extern HalfDuplexSerial Serial;
#endif
