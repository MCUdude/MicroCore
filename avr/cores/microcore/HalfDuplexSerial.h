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

#include "Arduino.h"
#include "Print.h"
#include "core_settings.h"


// Set default baud rate based on F_CPU
#ifndef CUSTOM_BAUD_RATE
  #if F_CPU >= 4800000L
    #define BAUD_RATE  115200
  #elif F_CPU >= 1000000L
    #define BAUD_RATE  38400
  #elif F_CPU >= 600000L
    #define BAUD_RATE  19200
  #else
    #define BAUD_RATE  300
    #error Clock speed too slow for serial communication!
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
  int RxByteNBNegOneReturn();          // NBNegOneReturn returns -1; which is the same as standard Serial.write()

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

class HalfDuplexSerial : public Print
{
  public:
    void begin(const uint32_t) { } // Does NOTHING, you have no need to call this, here only for compatibility
    void begin() { }               // Does NOTHING, you have no need to call this, here only for compatibility
    void end() { }                 // Does NOTHING, you have no need to call this, here only for compatibility
    int available(void) ;          // As we do not have a buffer, this always returns 0
    int peek(void)      ;          // As we do not have a buffer, this always returns -1
    void flush(void) { }           // Does NOTHING, you have no need to call this, here only for compatibility
    operator bool();               // Always returns true

    // Because we define our own write(uint8_t) (which incidentally is the case for EVERYTHING deriving
    // from stream) we have to pull the other write methods explicityly from Print::
    // because C++ isn't smart enough to work out that's what we want to do, or maybe it thinks we
    // are not smart enough to allow it to do that, either way, one of us is not smart enough.
    using Print::write; // pull in write(str) and write(buf, size) from Print

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

    int read(void);

    /** Read a byte, non-blocking, will optimize-out.
     *
     * This is exactly the same as read() except that it's not part of Stream:: so it can be
     * optimized out if unused, as a result it is available for you to use even if you
     * define HALF_DUPLEX_SERIAL_DISABLE_READ
     *
     * @return int  -1 for no-data-read, 0 or greater is the byte read
     */

    int read_byte(void);

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

    size_t  write(uint8_t ch);

    /** Write a byte, will optimize out.
     *
     * The same as write() except not virtual and so can be optimized out, as a result this is
     * available for you to use even if you set HALF_DUPLEX_SERIAL_DISABLE_WRITE
     *
     * @param ch Byte to write.
     * @return  Always returns 1
     */

    size_t write_byte(uint8_t ch);

};

extern HalfDuplexSerial Serial;
#endif
