# An Arduino core for the ATtiny13/ATtiny13A processors.

Fork from http://sourceforge.net/projects/ard-core13/
(http://forum.arduino.cc/index.php/topic,89781.0.html)

Includes some compilation errors fixes.

# Installation:

### OS X:
Copy attiny13 directory into /Applications/Arduino.app/Contents/Java/hardware/

### Windows:
Copy attiny13 directory into C:\arduino\arduino-\<version\>\hardware\


# Warning!

Default ArduinoISP sketch too fast for attiny13 @ 128 or 600 KHz ("Invalid device signature" error). 
To fix that, in ArduinoISP sketch change string:

\#define SPI_CLOCK (1000000/6)

with:

\#define SPI_CLOCK (128000/6)
