# MicroCore
A lightweight and heavily optimized Arduino core for the ATtiny13, ATtiny13A and ATtiny13V. This core requires at least Arduino IDE v1.6, where v1.6.5+ is recommended. <br/> This core is a heavily modified fork of the [at13 repository](https://github.com/bartgee/at13), which is again based on Smeezekitty's work on the [original ATtiny13 core](https://sourceforge.net/projects/ard-core13/). Still a lot of work have been put into this fork to make it up to date with todays requirements.
If you're into "pure" AVR programming, I'm happy to tell you that all relevant keywords are being highlighted by the IDE through a separate keywords file. Make sure to check out the [example files](https://github.com/MCUdude/MicroCore/tree/master/avr/libraries/AVR_examples/examples) (File > Examples > AVR C code examples).


# Table of contents
* [Why add Arduino support for these microcontrollers?](#why-add-arduino-support-for-these-microcontrollers)
* [Supported clock frequencies](#supported-clock-frequencies)
* [GCC flags](#gcc-flags)
* [BOD option](#bod-option)
* [Programmers](#programmers)
* [Core settings](#core-settings)
* [Working Arduino functions](#working-arduino-functions)
* [How to install](#how-to-install)
	- [Boards Manager Installation](#boards-manager-installation)
	- [Manual Installation](#manual-installation)
* [Pinout](#pinout)
* [Minimal setup](#minimal-setup)


## Why add Arduino support for these microcontrollers?
* They're SUPER cheap (we're talking cents here!)
* They come in both DIP and SOIC packages
* They're pin compatible with the ATtiny25/45/85
* You can still fit a lot of low level code into 1kb :wink:


##Supported clock frequencies
* 9.6 MHz internal oscillator (default)
* 4.8 MHz internal oscillator
* 1.2 MHz internal oscillator
* 600 kHz internal oscillator
* 128 kHz internal watchdog oscillator <b>*</b>

Select the ATtiny13 in the boards menu, then select the clock frequency. You'll have to hit "Burn bootloader" in order to set the correct fuses. Make sure you connect an ISP programmer, and select the correct one in the "Programmers" menu.
<b>*</b> Make sure to use one of the "slow" programmer options when using the 128 kHz option (e.g USBtinyISP (slow)).
</br></br>


##GCC flags
* -Os (default)
* -Os -flto
* -O1
* -O1 -flto
* -O3
* -O3 -flto
 
Compiler flags indicates what level of optimization the compiler should use. Just leave this on the default setting if you don't know what this is. If you want to learn more, head over to the [GNU GCC website](https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html).


##BOD option
Brown out detection, or BOD for short lets the microcontroller sense the input voltage and shut down if the voltage goes below the brown out setting. The ATtiny13 haven't dedicated the BOD selection to extended fuse, so there's no way to <b>only</b> change the BOD option in the Arduino IDE. If you need to change this setting you'll have to edit the boards.txt file.


##Programmers
MicroCore does not adds its own copies of all the standard programmers to the "Programmer" menu. Just select one of the stock programmers in the "Programmers" menu, and you're ready to "Burn Bootloader" or "Upload Using Programmer". The `externalprogrammers.txt` file is for use with [the Arduino Eclipse plugin](http://eclipse.baeyens.it), and will not appear under the "Programmers" menu in Arduino IDE.
 
Select your microcontroller in the boards menu, then select the clock frequency. You'll have to hit "Burn bootloader" in order to set the correct fuses and upload the correct bootloader. <br/>
Make sure you connect an ISP programmer, and select the correct one in the "Programmers" menu. 


##Core settings
To make sure you're able to fit your whole project into this tiny microcontroller and still be able to use Arduino functions, I've added some <b>core settings</b>. By modifying the [`core_settings.h`](https://github.com/MCUdude/MicroCore/blob/master/avr/cores/microcore/core_settings.h) file you can enable or disable core functions you need or don't need. For instance, you're able to save about 100 bytes of flash if you're willing to disable the millis()/micros() functions. 

If you know what you're doing and got full control, you can disable the safemode. For instance the safemode makes sure that PWM gets turned off is a pin drives high or low, or digital pins doesn't exeed the number 5 (6 digital pins in total). By disabling the safemode you'll gain some speed and flash space.


##Working Arduino functions
* [abs()](https://www.arduino.cc/en/Reference/Abs)
* [analogRead()](https://www.arduino.cc/en/Reference/AnalogRead)
* [analogWrite()](https://www.arduino.cc/en/Reference/AnalogWrite)
* [attachInterrupt()](https://www.arduino.cc/en/Reference/AttachInterrupt)
* [constrain()](https://www.arduino.cc/en/Reference/Constrain)
* [degrees()](https://github.com/MCUdude/MicroCore/blob/master/avr/cores/microcore/Arduino.h#L60)
* [delay()](https://www.arduino.cc/en/Reference/Delay)
* [delayMicroseconds()](https://www.arduino.cc/en/Reference/DelayMicroseconds)
* [detachInterrupt()](https://www.arduino.cc/en/Reference/DetachInterrupt)
* [digitalRead()](https://www.arduino.cc/en/Reference/DigitalRead)
* [digitalWrite()](https://www.arduino.cc/en/Reference/DigitalWrite)
* [max()](https://www.arduino.cc/en/Reference/Max)
* [map()](https://www.arduino.cc/en/Reference/Map)
* [min()](https://www.arduino.cc/en/Reference/Min)
* [micros()](https://www.arduino.cc/en/Reference/Micros)
* [millis()](https://www.arduino.cc/en/Reference/Millis)
* [pinMode()](https://www.arduino.cc/en/Reference/PinMode)
* [pulseIn()](https://www.arduino.cc/en/Reference/PulseIn)
* [radians()](https://github.com/MCUdude/MicroCore/blob/master/avr/cores/microcore/Arduino.h#L59)
* [random()](https://www.arduino.cc/en/Reference/Random)
* [randomSeed()](https://www.arduino.cc/en/Reference/RandomSeed)
* [round()](https://github.com/MCUdude/MicroCore/blob/master/avr/cores/microcore/Arduino.h#L58)
* [shiftIn()](https://www.arduino.cc/en/Reference/ShiftIn)
* [shiftOut()](https://www.arduino.cc/en/Reference/ShiftOut)
* [sq()](https://www.arduino.cc/en/Reference/Sq)


##How to install
#### Boards Manager Installation
A boards manager URL will be added later
~~This installation method requires Arduino IDE version 1.6.4 or greater.~~
* ~~Open the Arduino IDE.~~
* ~~Open the **File > Preferences** menu item.~~
* ~~Enter the following URL in **Additional Boards Manager URLs**: `https://mcudude.github.io/MicroCore/package_MCUdude_MicroCore_index.json`~~
* ~~Open the **Tools > Board > Boards Manager...** menu item.~~
* ~~Wait for the platform indexes to finish downloading.~~
* ~~Scroll down until you see the **MiniCore** entry and click on it.~~
  * ~~**Note**: If you are using Arduino IDE 1.6.6 then you may need to close **Boards Manager** and then reopen it before the **MiniCore** entry will appear.~~
* ~~Click **Install**.~~
* ~~After installation is complete close the **Boards Manager** window.~~


#### Manual Installation
Click on the "Clone or download" button in the upper right corner. Exctract the ZIP file, and move the extracted folder to the location "**~/Documents/Arduino/hardware**". Create the "hardware" folder if it doesn't exist.
Open Arduino IDE, and a new category in the boards menu called "MicroCore" will show up.


##Pinout
This diagram shows the pinout and the pheripherals of the ATtiny13. The Arduino pinout is directly mapped to the port number to minimize the code footprint.
<b>Click to enlarge:</b> 
</br> </br>
<img src="http://i.imgur.com/qREdVMb.jpg" width="800">




##Minimal setup
<br/>
<img src="http://i.imgur.com/p7mtBWp.png" width="600">
