# PlatformIO

[PlatformIO](https://platformio.org) is an open source ecosystem for embedded development. 
It has a built-in library manager and is Arduino compatible. It supports most operating systems; Windows, MacOS, Linux 32 and 64-bit, ARM and X86.  
And best of all, MicroCore is supported!

* [What is PlatformIO?](http://docs.platformio.org/en/latest/what-is-platformio.html)
* [PlatformIO IDE](http://platformio.org/#!/platformio-ide)
* Getting started with [PlatformIO IDE](http://docs.platformio.org/en/latest/ide/atom.html#quick-start) or [PlatformIO command line interface](http://docs.platformio.org/en/latest/quickstart.html)
* [Advanced functionality](http://docs.platformio.org/en/latest/platforms/atmelavr.html) 
* [Project Examples](http://docs.platformio.org/en/latest/platforms/atmelavr.html#examples)


## MicroCore + PlatformIO
MicroCore and PlatformIO goes great together. You can upload using your favorite programmer and print to the serial monitor. But you can also let PlatformIO calulate the fuses, just like Arduino IDE does!

PlatformIO uses the information provided in platformio.ini to calculate what fuse bits and what bootloader file to load.  
Simply provide enough information and run the following commands:  

```ini
; Set fuses
pio run -t fuses -e set_fuses
; (where "set_fuses" can be replace with a different environment to match your build configuration)
```

You can find a platformio.ini template you can use when creating a project for the ATtiny13 below.  
The most common functionality is available in this template. As you can see, the templated is divided into multiple environments.  

* The default build environment are defined under `[platformio]`.
* All parameters that are common for all environments are defined under `[env]`.
* Use the `[env:Upload_ISP]` to upload to your target.
* Use `[env:set_fuses]` to set the fuses.

More information on what each line means can be found futher down on this page.

``` ini
; PlatformIO template project configuration file for MicroCore
; https://github.com/MCUdude/MicroCore/
;
;   Build options: build flags, source filter
;   Hardware options: oscillator type, BOD, EEPROM retain
;   Upload options: programmer type, and extra flags
;   Library options: dependencies, extra library storages
;   Advanced options: extra scripting
;
; Please visit documentation for the other options
; https://github.com/MCUdude/MicroCore/blob/master/PlatformIO.md
; https://docs.platformio.org/page/projectconf.html
; https://docs.platformio.org/en/latest/platforms/atmelavr.html


[platformio]
default_envs = Upload_ISP ; Default build target


; Common settings for all environments
[env]
platform = atmelavr
framework = arduino

; TARGET SETTINGS
; Chip in use
board = ATtiny13
; Clock frequency in [Hz]
board_build.f_cpu = 9600000L

; BUILD OPTIONS
; Extra build flags
build_flags = 
; Uflag existing flags
build_unflags =

; SERIAL MONITOR OPTIONS
; Serial monitor port
monitor_port = /dev/cu.usberserial*
; Serial monitor baud rate
monitor_speed = 115200


; Run the following command to upload with this environment
; pio run -e Upload_ISP -t upload
[env:Upload_ISP]
; Custom upload procedure
upload_protocol = custom
; Avrdude upload flags
upload_flags =
  -C$PROJECT_PACKAGES_DIR/tool-avrdude/avrdude.conf
  -p$BOARD_MCU
  -PUSB
  -cusbasp
; Avrdude upload command
upload_command = avrdude $UPLOAD_FLAGS -U flash:w:$SOURCE:i


; Run the following command to set fuses
; pio run -e set_fuses -t fuses
[env:set_fuses]
board_hardware.oscillator = internal ; Oscillator type
board_hardware.bod = 2.7v            ; Set brown-out detection
board_hardware.eesave = yes          ; Preserve EEPROM when uploading using programmer
upload_protocol = usbasp             ; Use the USBasp as programmer
upload_flags =                       ; Select USB as upload port and divide the SPI clock by 8
  -PUSB
  -B8

```


### `board`
PlatformIO requires the `board` parameter to be present.

| Target     | Board name |
|------------|------------|
| ATtiny13/A | `ATtiny13` |


### `board_build.f_cpu`
Specifies the clock frequency in [Hz]. 
Used to determine what oscillator option to choose. A capital L has to be added to the end of the frequency number.
Below is a table with supported clocks for MicroCore. Defaults to 9.6 MHz if not specified.

| Clock speed | Oscillator | board_build.f_cpu         |
|-------------|------------|---------------------------|
| 20 MHz      | External   | `20000000L`               |
| 16 MHz      | External   | `16000000L`               |
| 12 MHz      | External   | `12000000L`               |
| 8 MHz       | External   | `8000000L`                |
| 1 MHz       | External   | `1000000L`                |
| 9.6 MHz     | Internal   | `9600000L` (defualt)      |
| 4.8 MHz     | Internal   | `4800000L`                |
| 1.2 MHz     | Internal   | `1200000L`                |
| 600 kHz     | Internal   | `600000L`                 |
| 128 kHz     | Internal   | `128000L`                 |


### `board_hardware.oscillator`
Spefices to use the internal or an external oscillator.  
Internal oscillator only works with `board_build.f_cpu` values  
`9600000L`, `4800000L`, `1200000L`, `600000L`, and `128000L`.

| Oscillator option    |
|----------------------|
| `internal` (default) |
| `external`           |


### `board_hardware.bod`
Specifies the hardware brown-out detection. Use `disabled` to disable.

| BOD option          |
|---------------------|
| `4.3v`              |
| `2.7v` (default)    |
| `1.8v`              |
| `disabled`          |


### `board_hardware.eesave`
Specifies if the EEPROM memory should be retained when uploading using a programmer. Use `no` to disable.

| EEPROM retain   |
|-----------------|
| `yes` (default) |
| `no`            |


### `build_flags`
This parameter is used to set compiler flags. You can use this field to change the core settings rather than modifying core_settings.h like you would have to in Arduino IDE.  
Here is a list of some settings you can change:

| Flag                                                                                                                                                            | Description                                                                                                                                                                                                          |
|-----------------------------------------------------------------------------------------------------------------------------------------------------------------|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| -DDISABLE_SAFEMODE                                                                                                                                              | Disables safemode, which saves space but are not as "safe". However, If you know what you're doing, like explicitly writing a pin as an output before using the analogWrite() function, you can save a little space. |
| -DADC_PRESCALER_2 <br/>-DADC_PRESCALER_4 <br/>-DADC_PRESCALER_8 <br/>-DADC_PRESCALER_16 <br/>-DADC_PRESCALER32 <br/>-DADC_PRESCALER_64 <br/>-DADC_PRESCALER_128 | Set the ADC prescaler. If not defined, MicroCore will select the best one based on the F_CPU. Use only one of these flags.                                                                                           |
| -DPWM_PHASE_CORRECT                                                                                                                                             | Use phase correct PWM generation. Uses fast PWM if not defined                                                                                                                                                       |
| -DUART_TX_PIN=B,1                                                                                                                                               | Change the default UART TX pin. If not defined, it will default to B,0                                                                                                                                               |
| -DUART_RX_PIN=B,0                                                                                                                                               | Change the default UART RX pin. If not defined, it will default to B,1                                                                                                                                               |
| -DINTERRUPT_SERIAL_RX                                                                                                                                           | Use interrupt based (PCINT) UART receive                                                                                                                                                                             |
| -DCUSTOM_BAUDRATE=19200                                                                                                                                         | Manually set UART baudrate (for `Serial.print()`).<br/>See table in below for a table of all supported baud rates vs F_CPU.                                                                                          |

**Example:**
`build_flags = -DDPWM_PHASE_CORRECT -DDISABLE_SAFEMODE`


### `build_unflags`
This parameter is used to unflag. If you for some reason don't want to use LTO, you can unflag it like so:
```
build_unflags = -flto
```


### `upload_protocol`
Specified the Avrdude compatible programmer.  
Note that you can divide down the programmer clock speed by adding the -B flag: `-B8` will give you 1/8th speed.  
Here's a list of some popular programmers:

| Programmer name | Avrdude name | Extra flags needed | Additinal information                          |
|-----------------|--------------|--------------------|------------------------------------------------|
| USBasp          | `usbasp`     |                    |                                                |
| USBtinyUSP      | `usbtiny`    |                    |                                                |
| Arduino as ISP  | `stk500v1`   | `-b19200`          | Serial port must be specified in `upload_port` |
| AVRISPmkII      | `avrispmkii` |                    |                                                |


### `upload_port`
Specified the programmer serial port.  
Only needed for UART based programmers such as the Arduino as ISP (stk500v1).


### `upload_flags`
Specifies extra Avrdude flags. Note that each flag has to be on their separate line:
```
upload_flags = -b19200
-B8
-v
```


### `monitor_port`
PlatformIO detects serial ports automatically. However, if you want to override this you can uncomment `monitor_port`. Use `/dev/[port]` on Unix compatible systems, and use `COMx` on Windows.  
If you're having trouble connecting to the serial port on Windows, try `\\.\COMx` instead.


### `monitor_speed`
Sets the serial monitor baud rate. Defaults to 9600 if not defined.


## Supported baudrates
Here is a list of all supported baudrates.  
See the [build_flags](#build_flags) section on how you can change the baud rate instead of using the default one:

| Clock & baud | 460800 | 250000 | 230400 | 115200   | 57600    | 38400 | 19200    | 9600     | 4800 | 2400 | 1200 |
|--------------|--------|--------|--------|----------|----------|-------|----------|----------|------|------|------|
| 20 MHz       | X      | X      | X      | X (def.) | X        |       |          |          |      |      |      |
| 16 MHz       | X      | X      | X      | X (def.) | X        | X     |          |          |      |      |      |
| 12 MHz       |        | X      | X      | X (def.) | X        | X     |          |          |      |      |      |
| 9.6 MHz      |        | X      | X      | X (def.) | X        | X     |          |          |      |      |      |
| 8 MHz        |        | X      | X      | X (def.) | X        | X     |          |          |      |      |      |
| 4.8 MHz      |        | X      | X      | X        | X (def.) | X     | X        | X        |      |      |      |
| 1.2 MHz      |        |        |        |          | X        | X     | X (def.) | X        | X    | X    |      |
| 1 MHz        |        |        |        |          |          | X     | X (def.) | X        | X    | X    |      |
| 600 kHz      |        |        |        |          |          |       | X        | X (def.) | X    | X    | X    |
| 128 kHz      |        |        |        |          |          |       |          |          |      |      |      |
