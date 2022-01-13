/***********************************************************************|
| Capacitive touch library for ATtiny13                                 |
|                                                                       |
| Touch_read.ino                                                        |
|                                                                       |
| A light-weight library for using the ADC inputs on the ATtiny13 as    |
| capacitive touch sensors.                                             |
| Developed by MCUdude, based on nerdralph's adctouch implementation.   |
| https://github.com/MCUdude/MicroCore                                  |
|                                                                       |
| In this example we use A2 and A3 as touch sensor pins, and prints out |
| the boolean and the actual measurement values to the serial monitor.  |
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

#include <ADCTouch.h>
#include <EEPROM.h>

// Sample each touch pin 32 times
const uint16_t ADCTouch::samples = 32;

// Touch reference values to zero out the offset
int16_t ref_A2;
int16_t ref_A3;

// Touch threshold for turning on or off the LEDs
// Lower is more sensitive
const uint8_t threshold = 50;


void setup()
{
  uint8_t cal = EEPROM.read(0);
  if (cal < 0x80)
    OSCCAL = cal;
  Serial.begin();

  // Reference values to account for the capacitance of the touch pad
  ref_A2 = Touch.read(A2);
  ref_A3 = Touch.read(A3);
}

void loop()
{
  // Read touch
  int16_t val_A2 = Touch.read(A2);
  int16_t val_A3 = Touch.read(A3);

  // Drift compensation
  if(val_A2 > ref_A2)
    ref_A2++;
  else
    ref_A2 = (val_A2 + ref_A2)/2;
  if(val_A3 > ref_A3)
    ref_A3++;
  else
    ref_A3 = (val_A3 + ref_A3)/2;

  // Remove offset
  val_A2 -= ref_A2;
  val_A3 -= ref_A3;

  Serial.print(F("A2 touch bool: "));
  Serial.print(val_A2 > threshold); // Send (boolean) pressed or not pressed
  Serial.print(F(", A2 touch value: "));
  Serial.print(val_A2);
  Serial.print(F(", A2 ref value: "));
  Serial.print(ref_A2);

  Serial.print(F("\tA3 touch bool: "));
  Serial.print(val_A3 > threshold); // Send (boolean) pressed or not pressed
  Serial.print(F(", A3 touch value: "));
  Serial.print(val_A3);
  Serial.print(F(", A3 ref value: "));
  Serial.print(ref_A3);
  Serial.print('\n');
  delay(100);
}
