# An Arduino core for the ATtiny13/ATtiny13A processors for USBasp.

Fork from http://sourceforge.net/projects/ard-core13/
(http://forum.arduino.cc/index.php/topic,89781.0.html)

Includes some compilation errors fixes.

Includes changes made by aleksey_ua from Arduino Forum.
(http://forum.arduino.cc/index.php?topic=89781.msg2363643#msg2363643)

# Installation:

### Linux:
Copy attiny13 directory to /opt/arduino-1.6.7/hardware/.

### OS X:
Copy attiny13 directory into /Applications/Arduino.app/Contents/Java/hardware/.

### Windows:
Copy attiny13 directory into C:\arduino\arduino-\<version\>\hardware\.


# Warning!

Default ArduinoISP sketch too fast for attiny13 @ 128 or 600 KHz ("Invalid device signature" error). 
To fix that, in ArduinoISP sketch change string:

\#define SPI_CLOCK (1000000/6)

with:

\#define SPI_CLOCK (128000/6)

# What to do if you can't program your attiny13a MCU

If you've got a brand new attiny13a you probably have to set proper fuse bits
before you start programming your chip. It's just simple one line command - see
details in fusebits-set-attiny13a.txt. I couldn't even burn the bootloader
before I set proper fuse bits for my MCU.
