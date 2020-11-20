/*
  ATtiny13 internal oscillator tuner

  By Ralph Doncaster (2020)
  Tweaked and tuned by MCUdude
  ------------------------------------------------------------------------------

  [ See diagram: https://github.com/MCUdude/MicroCore#minimal-setup ]

  Tunes the internal oscillator using a software serial implementation
  and store the calibrated value to EEPROM address 0.

  Start off by opening the serial monitor and select the correct baud rate.
  Make sure you're not sending any line ending characters (CR, LF).
  Repedeatly press 'x' [send] to tune the internal oscillator. After a few
  messages you'll eventually see readable text in the serial monitor, and a
  new, stable OSCCAL value. Continue doing this until you'll get a message
  saying that the value have been stored to EEPROM address 0. After this the
  program will halt.

  RECOMMENDED SETTINGS FOR THIS SKETCH
  ------------------------------------------------------------------------------

  Tools > Board          : ATtiny13
  Tools > BOD            : [Use any BOD level you like]
  Tools > Clock          : 9.6 MHz or 4.8 MHz depending on what osc. to tune
  Tools > Timing         : Micros disabled

  SERIAL REMINDER
  ------------------------------------------------------------------------------
  The baud rate is IGNORED on the ATtiny13 due to using a simplified serial.
  The actual Baud Rate used is dependant on the processor speed.
  Note that you can specify a custom baud rate if the following ones does
  not fit your application.

  THESE CLOCKS USES 115200 BAUD:   THIS CLOCK USES 57600 BAUD:
  (External)  20 MHz               (Internal) 4.8 MHz
  (External)  16 MHz
  (External)  12 MHz
  (External)   8 MHz
  (Internal) 9.6 MHz

  THESE CLOCKS USES 19200 BAUD:    THIS CLOCK USES 9600 BAUD:
  (Internal) 1.2 MHz               (Internal) 600 KHz
  (External)   1 MHz

  If you get garbage output:
   1. Check baud rate as above
   2. Check if you have anything else connected to TX/RX like an LED
   3. You haven't sent enough 'x' characters yet
*/

#include <EEPROM.h>

// Add 0.5 for integer rounding
const uint8_t CYCLES_PER_BIT = (uint8_t)(PUBIT_CYCLES + 0.5);

// converts 4-bit nibble to ascii hex
uint8_t nibbletohex(uint8_t value)
{
  value &= 0x0F;
  if ( value > 9 )
    value += 'A' - ':';
  return value + '0';
}

void printHex(uint8_t value)
{
  putx(nibbletohex(value/16));
  putx(nibbletohex(value));
}

// RX interrupt
ISR(PCINT0_vect)
{
  static uint8_t cal_counter;
  
  // Start timer when pin transitions low
  if ((PINB & _BV(PURXBIT)) == 0)
    TCCR0B = _BV(CS00);
  else
  {
    uint8_t current = TCNT0;    
    // End of interval, reset counter
    TCCR0B = 0;
    TCNT0 = 0;

    // 'x' begins with 3 zeros + start bit = 4 * bit-time
    // Match speed to soft uart bit time
    // Use mod256 math to handle potential uint8_t overflow
    uint16_t expected = (uint16_t)(PUBIT_CYCLES * 4 + 0.5);
    int8_t delta = (expected & 0xFF) - current;
    prints(F("OSCCAL: 0x"));
    printHex(OSCCAL);
    if (delta > 3)
    {
      OSCCAL++;
      prints(F(" - slow\n"));
    }
    else if (delta < -3)
    {
      OSCCAL--;
      prints(F(" - fast\n"));
    }
    else
    {
      prints(F(" - good\n"));
      cal_counter++;
    }
  }

  // Store new OSCCAL to EEPROM when stable    
  if(cal_counter >= 5)
  {
    EEPROM.write(0, OSCCAL);
    prints(F("New OSCCAL stored to EEPROM addr. 0\n"));
    while(true);
  }
  
  // Clear interrupt flag in case another triggered
  GIFR = _BV(PCIF);
}

void setup()
{
  // Serial monitor open delay
  delay(500);
  
  // Pullup RX line
  PORTB |= _BV(PURXBIT);

  printHex(OSCCAL);
  prints(F(" Hit x to test.\n"));

  wait_x:
  // Wait for tuning character to ensure not reading noise
  // before entering tuning mode
  uint8_t counter = 0;
  while (PINB & _BV(PURXBIT));
  do 
  {
    counter++;
  } while ((PINB & _BV(PURXBIT)) == 0);
  
  // Low period should be 4 bit-times for 'x'
  // Counter loop is 4 cycles, so counter =~ cycles/bit
  // 1/4 = 25% timing margin
  uint8_t margin = CYCLES_PER_BIT / 4;
  uint8_t delta = __builtin_abs(counter - CYCLES_PER_BIT);
  if (delta > margin)
  {
    prints(F("Noise skipped\n"));
    goto wait_x;
  }
  
  // Skip remaining bits in frame
  delay(1);

  prints(F(" OSCCAL "));
  if (delta < 2)
  { 
    prints(F("OK\n"));
    return;
  }
  prints(F("Imperfect\n"));
  
  // Reset counter for first interrupt
  TCCR0B = 0;
  TCNT0 = 0;

  // Prepare RX interrupt
  PCMSK = _BV(PURXBIT);
  GIMSK = _BV(PCIE);

  // Enable interrupts
  sei();

  // Prepare sleep
  MCUCR |= _BV(SE);
}

void loop() 
{
  // Nothing here really...
  asm("sleep");
}