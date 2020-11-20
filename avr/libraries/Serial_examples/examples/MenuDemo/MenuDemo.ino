/*
  Print menu to Serial, read selection from Serial

  By J.Sleeman (sparks@gogo.co.nz)
  MicroCore support by MCUdude
  ------------------------------------------------------------------------------

  [ See diagram: https://github.com/MCUdude/MicroCore#minimal-setup ]

  Gives you some options, allows you to pick from them by sending a single
  character.  Blocks until you choose.

  RECOMMENDED SETTINGS FOR THIS SKETCH
  ------------------------------------------------------------------------------

  Tools > Board          : ATtiny13
  Tools > BOD            : [Use any BOD level you like]
  Tools > Clock          : [Use any clock except 128 kHz]
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
   3. Check OSCCAL (see OSCCAL tuner example)
*/

#include <EEPROM.h>

void setup() 
{
  // Check if there exist any OSCCAL value in EEPROM addr. 0
  // If not, run the oscillator tuner sketch first
  uint8_t cal = EEPROM.read(0);
  if(cal < 0x7F)
    OSCCAL = cal;

  // Note that any baud rate specified is ignored on the ATtiny13. See header above.
  Serial.begin();
}

void loop() 
{
  Serial.println(F("\n~~ Tiny Menu ~~\n"));  
  Serial.println(F("1. Say Hello"));
  Serial.println(F("2. Say Goodbye"));
  Serial.println(F("3. Sing"));
  Serial.println(F("4. Jack"));

  switch(Serial.read_blocking())
  {
    case '1':
      Serial.println(F(">> Hello"));
      break;
    case '2':
      Serial.println(F(">> Goodbye"));
      break;
    case '3':
      Serial.println(F(">> Daisy, daisy, give me your answer do."));
      break;
    case '4':
      for(uint8_t i = 1; i <= 100; i++)
      {
        Serial.print(i);
        Serial.println(F(": All work and no play makes Jack a dull boy."));       
      }
      break;
    default:
      Serial.println(F("* Unkown Command *"));
      break;
  }
  delay(1000);
}
