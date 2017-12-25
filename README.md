# MicroCore
A lightweight Arduino hardware package for ATtiny13, ATtiny13A and ATtiny13V. This core is a complete rewrite of Smeezekitty's [core13](https://sourceforge.net/projects/ard-core13/). A lot of work has been put into MicroCore to make it up to date with the latest requirements.
If you're into "pure" AVR programming, I'm happy to tell you that all relevant keywords are being highlighted by the IDE through a separate keywords file. Make sure to check out the [example files](https://github.com/MCUdude/MicroCore/tree/master/avr/libraries/AVR_examples/examples) (File > Examples > AVR C code examples).


## Table of contents
* [Why use the ATtiny13 in an Arduino project?](#why-use-the-attiny13-in-an-arduino-project)
* [Supported clock frequencies](#supported-clock-frequencies)
* [LTO](#lto)
* [BOD option](#bod-option)
* [Programmers](#programmers)
* **[Core settings](#core-settings)**
* **[How to install](#how-to-install)**
	- [Boards Manager Installation](#boards-manager-installation)
	- [Manual Installation](#manual-installation)
* **[Getting started with MicroCore](#getting-started-with-microcore)**	
* [Pinout](#pinout)
* [Minimal setup](#minimal-setup)
* [Working Arduino functions](#working-arduino-functions)


## Why use the ATtiny13 in an Arduino project?
* They're DIRT cheap (we're talking cents here!)
* They come in both DIP and SOIC packages
* They're pin compatible with the ATtiny25/45/85 family and often code compatible
* Most of the [Arduino functions](https://www.arduino.cc/reference/en/) is implemented in MicroCore
* Thanks to MicroCore you can fit a lot of high level code into 1024 bytes!


## Supported clock frequencies
The ATtiny13 has several internal oscillators, and these are the available clock frequencies:
* 9.6 MHz internal oscillator *(default)*
* 4.8 MHz internal oscillator
* 1.2 MHz internal oscillator
* 600 kHz internal oscillator
* 128 kHz internal watchdog oscillator <b>*</b>

If you want other or higher clock frequencies, you can apply an external clock source. Note that the ATtiny13 requires an external clock signal, and is not able to drive a resonator circuit itself. You may use a [quartz crystal oscillator](https://en.wikipedia.org/wiki/Crystal_oscillator#/media/File:18MHZ_12MHZ_Crystal_110.jpg).
Supported external clock frequencies:
* 20 MHz external oscillator
* 26 MHz external oscillator
* 12 MHz external oscillator
* 8 MHz external oscillator
* 1 MHz external oscillator

Select the ATtiny13 in the boards menu, then select the clock frequency. You'll have to hit "Burn bootloader" in order to set the correct fuses. Make sure you connect an ISP programmer, and select the correct one in the "Programmers" menu. <br/> <br/>
<b>*</b> Make sure to use one of the "slow" programmer options when using the 128 kHz option (e.g _USBtinyISP (slow)_).
</br></br>


## LTO
LTO or link time optimization is enabled by default, and reduces the code size at compile time. If you want to learn more about compiler flags and link time optimization (LTO), head over to the [GNU GCC website](https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html). Ralph Doncaster has also written a [great post about LTO](http://nerdralph.blogspot.no/2014/04/gcc-link-time-optimization-can-fix-bad.html) you should read.<br/>
Compiler optimization can certainly make your code smaller in size. Still, it's all about writing efficient code. Atmel have created an application note on how to write more efficient C code for AVR microcontrollers.
This is great knowledge, so you should absolutely check it out - [AVR4027: Tips and Tricks to Optimize Your C Code for 8-bit AVR Microcontrollers](http://www.atmel.com/images/doc8453.pdf).


## BOD option
Brown out detection, or BOD for short lets the microcontroller sense the input voltage and shut down if the voltage goes below the brown out setting.
These are the available BOD options:
* 4.3 v
* 2.7 v
* 1.8 v
* Disabled


## Programmers
When the ATtiny13 is running from the internal 128 kHz oscillator, it's too slow to interact with the programming tool. That's why this core adds some additional programmers to the list, with the suffix *(slow)*. These options makes the programmers run at a lower clock speed, so the microcontroller can keep up.
The `externalprogrammers.txt` file is for use with [the Arduino Eclipse plugin](http://eclipse.baeyens.it), and will not appear under the "Programmers" menu in the Arduino IDE.
 
Select your microcontroller in the boards menu, then select the clock frequency. You'll have to hit "Burn bootloader" in order to set the correct fuses and upload the correct bootloader. <br/>
Make sure you connect an ISP programmer, and select the correct one in the "Programmers" menu. 


## Core settings
To make sure you're able to fit your whole project into this tiny microcontroller and still be able to use Arduino functions, I've added some <b>core settings</b>. By modifying the [`core_settings.h`](https://github.com/MCUdude/MicroCore/blob/master/avr/cores/microcore/core_settings.h) file you can enable or disable core functions you need or don't need. 
For instance, you save about 100 bytes of flash if you're willing to disable the millis() function. 
If you know what you're doing and want full control, you can disable the safemode. For instance safemode makes sure that PWM gets turned off if a pin drives high or low, or digital pins don't exceed the number 5 (6 digital pins in total). By disabling safemode you'll gain some speed and flash space.


## How to install
#### Boards Manager Installation
This installation method requires Arduino IDE version 1.6.4 or greater.
* Open the Arduino IDE.
* Open the **File > Preferences** menu item.
* Enter the following URL in **Additional Boards Manager URLs**: `https://mcudude.github.io/MicroCore/package_MCUdude_MicroCore_index.json`
* Open the **Tools > Board > Boards Manager...** menu item.
* Wait for the platform indexes to finish downloading.
* Scroll down until you see the **MicroCore** entry and click on it.
* Click **Install**.
* After installation is complete close the **Boards Manager** window.


#### Manual Installation
Click on the "Clone or download" button in the upper right corner. Extract the ZIP file, and move the extracted folder to the location "**~/Documents/Arduino/hardware**". Create the "hardware" folder if it doesn't exist.
Open Arduino IDE, and a new category in the boards menu called "MicroCore" will show up.


## Getting started with MicroCore
Ok, so you have downloaded and installed MicroCore, but how do you get the wheels spinning? Here's a quick start guide:
* Hook up your microcontroller as shown in the [pinout diagram](#pinout).
* Open the **Tools > Board** menu item, and select ATtiny13.
* Select your prefered BOD option. Read more about BOD [here](#bod-option).
* Select your prefered clock frequency. **9.6 MHz internal oscillator** is the default setting. Do not use the external oscillator option if you don't have an external clock source. Remember that a regular two pin crystal will not work on the ATtiny13.
* If you want you can change the compiler flags for further optimization. Leave this on the default setting if you don't know what compiler flags are. 
* Select what kind of programmer you're using under the **Programmers** menu. Use one of the **slow** programmers if you're using the 128 kHz oscillator option, e.g **USBtinyISP (slow)**.
* Hit **Burn Bootloader** to burn the fuses. The "settings" are now stored on the microcontroller!
* Now that the correct fuse settings is sat you can upload your code by using your programmer tool. Simply hit *Upload*, and the code will be uploaded to the microcontroller.
* If you want to do some changes; change the BOD option for instance, you'll have to hit **Burn Bootloader** again.


## Pinout
This diagram shows the pinout and the peripherals of ATtiny13. The Arduino pinout is directly mapped to the port number to minimize code footprint.
<b>Click to enlarge:</b> 
</br> </br>
<img src="http://i.imgur.com/JsbguPV.jpg" width="800">


## Minimal setup
<br/>
<img src="http://i.imgur.com/SjCN7oZ.png" width="600">


## Working Arduino functions

#### Arduino functions
* [abs()](https://www.arduino.cc/en/Reference/Abs)
* [analogRead()](https://www.arduino.cc/en/Reference/AnalogRead)
* [analogWrite()](https://www.arduino.cc/en/Reference/AnalogWrite)
* [attachInterrupt()](https://www.arduino.cc/en/Reference/AttachInterrupt)
* [bit()](https://www.arduino.cc/en/Reference/Bit)
* [bitClear()](https://www.arduino.cc/en/Reference/BitClear)
* [bitRead()](https://www.arduino.cc/en/Reference/BitRead)
* [bitSet()](https://www.arduino.cc/en/Reference/BitSet)
* [bitWrite()](https://www.arduino.cc/en/Reference/BitWrite)
* [constrain()](https://www.arduino.cc/en/Reference/Constrain)
* [degrees()](https://github.com/MCUdude/MicroCore/blob/83b192f46fa5c58f8b7a60d7a678b2199d167ba5/avr/cores/microcore/Arduino.h#L56)
* [delay()](https://www.arduino.cc/en/Reference/Delay)
* [delayMicroseconds()](https://www.arduino.cc/en/Reference/DelayMicroseconds)   *Macro wrapper for _delay_us() (inline function)*
* [detachInterrupt()](https://www.arduino.cc/en/Reference/DetachInterrupt)
* [digitalRead()](https://www.arduino.cc/en/Reference/DigitalRead)
* [digitalWrite()](https://www.arduino.cc/en/Reference/DigitalWrite)
* [highByte()](https://www.arduino.cc/en/Reference/HighByte)
* [interrupts()](https://www.arduino.cc/en/Reference/Interrupts)
* [lowByte()](https://www.arduino.cc/en/Reference/LowByte)
* [map()](https://www.arduino.cc/en/Reference/Map)
* [max()](https://www.arduino.cc/en/Reference/Max)
* [min()](https://www.arduino.cc/en/Reference/Min)
* [micros()](https://www.arduino.cc/en/Reference/Micros)   *Must be manually enabled in the [core settings](https://github.com/MCUdude/MicroCore/blob/master/avr/cores/microcore/core_settings.h)*
* [millis()](https://www.arduino.cc/en/Reference/Millis)   *Watchdog timer based. Will increase with steps of 16*
* [noInterrupts()](https://www.arduino.cc/en/Reference/NoInterrupts)
* [noTone()](https://www.arduino.cc/en/Reference/NoTone)
* [pinMode()](https://www.arduino.cc/en/Reference/PinMode)
* [pow()](https://www.arduino.cc/en/Reference/Pow)
* [pulseIn()](https://www.arduino.cc/en/Reference/PulseIn)   *micros() must be enabled in the [core settings](https://github.com/MCUdude/MicroCore/blob/master/avr/cores/microcore/core_settings.h) first* 
* [radians()](https://github.com/MCUdude/MicroCore/blob/83b192f46fa5c58f8b7a60d7a678b2199d167ba5/avr/cores/microcore/Arduino.h#L55)
* [random()](https://www.arduino.cc/en/Reference/Random)
* [randomSeed()](https://www.arduino.cc/en/Reference/RandomSeed)
* [round()](https://github.com/MCUdude/MicroCore/blob/83b192f46fa5c58f8b7a60d7a678b2199d167ba5/avr/cores/microcore/Arduino.h#L54)
* [shiftIn()](https://www.arduino.cc/en/Reference/ShiftIn)
* [shiftOut()](https://www.arduino.cc/en/Reference/ShiftOut)
* [sizeof()](https://www.arduino.cc/en/Reference/Sizeof)
* [stopTone](https://github.com/MCUdude/MicroCore/blob/master/avr/cores/microcore/Tone.cpp)
* [sq()](https://www.arduino.cc/en/Reference/Sq)
* [sqrt()](https://www.arduino.cc/en/Reference/Sqrt)
* [tone()](https://www.arduino.cc/en/Reference/Tone)

