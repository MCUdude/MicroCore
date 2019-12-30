/*
  ATtiny13 internal oscillator tuner

  By Ralph Doncaster (2019)
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

const int8_t delta_val = 5;
const uint8_t uart_rx_pin = 1;

// Converts 4-bit nibble to ascii hex
uint8_t nibbletohex(uint8_t value)
{
  value &= 0x0F;
  if (value > 9)
    value += 'A' - ':';
  return value + '0';
}

// Function to run when Rx interrupt occurs
void rxInterrupt()
{
  // Start timer when pin transitions low
  if ((PINB & _BV(uart_rx_pin)) == 0)
    TCCR0B = _BV(CS00);
  else
  {
    uint8_t current = TCNT0;
    // End of interval, reset counter
    TCCR0B = 0;
    TCNT0 = 0;
    // The 'x' character begins with 3 zeros + start bit = 4
    // Match speed to soft uart timing of 7 + TXDELAYCONT * 3
    uint8_t expected = (uint8_t)(4 * (7 + TXDELAYCOUNT * 3));
    int8_t delta = expected - current;
    if (delta > delta_val)  OSCCAL++;
    if (delta < -delta_val) OSCCAL--;
    asm("lpm"); // 3 cycle delay

    // Print calculation
    Serial.print(F("Delta: 0x"));
    Serial.write(nibbletohex(delta >> 4));
    Serial.write(nibbletohex(delta));
    Serial.print(F("  New cal: 0x"));
    Serial.write(nibbletohex(OSCCAL >> 4));
    Serial.write(nibbletohex(OSCCAL));
    Serial.write('\n');
    
    // Store new OSCCAL to EEPROM when stable
    static uint8_t cal;
    static uint8_t cal_counter;
    if(cal != OSCCAL)
      cal = OSCCAL;
    else if(++cal_counter >= 16)
    {
      EEPROM.write(0, OSCCAL);
      Serial.print(F("New OSCCAL stored to EEPROM addr. 0\n"));
      while(true);
    }
  }
 
  // Clear interrupt flag in case another triggered
  GIFR = _BV(INTF0);
}

void setup() 
{
  // Note that any baud rate specified is ignored on the ATtiny13. See header above.
  Serial.begin();

  // Prepare for sleep mode
  MCUCR = _BV(SE);

  // Enable Rx pin pullup
  digitalWrite(uart_rx_pin,HIGH);
  
  // Setup RX interrupt
  attachInterrupt(0, rxInterrupt, CHANGE);
  
  delay(1000);

  // Print default message
  Serial.write('x');
  Serial.write('\n');
  
  wait_x:
  // Wait for tuning character to ensure not reading noise
  // before entering tuning mode
  uint8_t counter = 0;
  while (PINB & _BV(uart_rx_pin));
  do 
  {
    counter++;
  } while ((PINB & _BV(uart_rx_pin)) == 0);

  // Low period should be 4 bit-times
  uint8_t margin = BIT_CYCLES/8;
  if (counter - (uint8_t)BIT_CYCLES > margin)
    goto wait_x;
  else
  {
    if ((uint8_t)BIT_CYCLES - counter > margin)
      goto wait_x;
  }

  delay(1); // Skip remaining bits in frame

  // Reset counter for first interrupt
  TCCR0B = 0;
  TCNT0 = 0;
}

void loop() 
{
  // Nothing here really...
  asm("sleep");
}