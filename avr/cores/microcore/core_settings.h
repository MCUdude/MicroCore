/*** MicroCore - core_settings.h ***
Arduino core designed for ATtiny13
File created and maintained by MCUdude
https://github.com/MCUdude/MicroCore

This file lets you turn core functions on and off.
Since the ATtiny13 is really short on space, you
might want to disable some functions you don't
need in order to free up space.
*/

#ifndef core_settings_h
#define core_settings_h

// These defintions is used when compiling Serial printing. Serial is bitbanged, and may be tuned here
// to reduce the compiled size. The default settings allows you to do pretty much exactly what an "ordinary"
// Arduino would support.

// Pins to use for Tx and Rx
// Note that you can use the same pin for TX and RX by giving them the same number
#ifndef UART_TX_PIN
  #define UART_TX_PIN B,0
#endif
#ifndef UART_RX_PIN
  #define UART_RX_PIN B,1
#endif

// Use PCINT interrupts for receiving serial data
//#define INTERRUPT_SERIAL_RX


// What should be the largest supported number?
#ifndef PRINT_MAX_INT_TYPE
  #define PRINT_MAX_INT_TYPE PRINT_INT_TYPE_LONG // PRINT_INT_TYPE_INT, PRINT_INT_TYPE_BYTE and PRINT_INT_TYPE_LONG supported.
#endif


// What bases should be supported?
#define PRINT_USE_BASE_BIN
#define PRINT_USE_BASE_DEC
#define PRINT_USE_BASE_HEX
//#define PRINT_USE_BASE_OCT


// Baudrate is automatically selected based on F_CPU. If a different speed is needed, BAUD_RATE can be defined
// to reflect this.
// See table below for all supported baud rates. Note that when using interrupt based RX, supported RX speed
// will be lower.

//#define CUSTOM_BAUD_RATE 19200

// | Clock & baud | 460800 | 250000 | 230400 | 115200   | 57600    | 38400 | 19200    | 9600     | 4800 | 2400 | 1200 |
// |--------------|--------|--------|--------|----------|----------|-------|----------|----------|------|------|------|
// | 20 MHz       | X      | X      | X      | X (def.) | X        |       |          |          |      |      |      |
// | 16 MHz       | X      | X      | X      | X (def.) | X        | X     |          |          |      |      |      |
// | 12 MHz       |        | X      | X      | X (def.) | X        | X     |          |          |      |      |      |
// | 9.6 MHz      |        | X      | X      | X (def.) | X        | X     |          |          |      |      |      |
// | 8 MHz        |        | X      | X      | X (def.) | X        | X     |          |          |      |      |      |
// | 4.8 MHz      |        | X      | X      | X        | X (def.) | X     | X        | X        |      |      |      |
// | 1.2 MHz      |        |        |        |          | X        | X     | X (def.) | X        | X    | X    |      |
// | 1 MHz        |        |        |        |          |          | X     | X (def.) | X        | X    | X    |      |
// | 600 kHz      |        |        |        |          |          |       | X        | X (def.) | X    | X    | X    |
// | 128 kHz      |        |        |        |          |          |       |          |          |      |      |      |


// Makes the core "idiot proof" (see wiring_digital.c and wiring_pwm.h for examples where SAFEMODE is used).
// Enabling SAFEMODE takes up more flash space, but "makes sure" you don't screw up, like by reading the
// state of a pin while it's outputting a PWM signal. If you know what you're doing, like explicitly writing
// a pin as an output before using the analogWrite() function, you can save a lot of space by disabling this.
#ifndef DISABLE_SAFEMODE // Trigger for PlatformIO
  #define SAFEMODE
#endif


// This is the ADC settings
// Here the ADC prescaler can be changed if needed.
// Note that the sample rate should exceed 200 kHz
#define ADC_PRESCALER_AUTO    // Selects the "best suited" prescaler based on F_CPU
//#define ADC_PRESCALER_2     // ADC sample rate = (F_CPU/13) / 2
//#define ADC_PRESCALER_4     // ADC sample rate = (F_CPU/13) / 4
//#define ADC_PRESCALER_8     // ADC sample rate = (F_CPU/13) / 8
//#define ADC_PRESCALER_16    // ADC sample rate = (F_CPU/13) / 16
//#define ADC_PRESCALER_32    // ADC sample rate = (F_CPU/13) / 32
//#define ADC_PRESCALER_64    // ADC sample rate = (F_CPU/13) / 64
//#define ADC_PRESCALER_128   // ADC sample rate = (F_CPU/13) / 128


// Here's the PWM settings for Timer0
// These settings will also be disabled if SETUP_PWM is commented out
// Note that ENABLE_MICROS will override this setting
#define PWM_PRESCALER_AUTO    // Selects the "best suited" prescaler based on F_CPU
//#define PWM_PRESCALER_NONE  // PWM frequency = (F_CPU/256) / 1
//#define PWM_PRESCALER_8     // PWM frequency = (F_CPU/256) / 8
//#define PWM_PRESCALER_64    // PWM frequency = (F_CPU/256) / 64
//#define PWM_PRESCALER_256   // PWM frequency = (F_CPU/256) / 256
//#define PWM_PRESCALER_1024  // PWM frequency = (F_CPU/256) / 1024


// These are the waveform generation settings for timer0
#define PWM_FAST          //  <-- DEFAULT
//#define PWM_PHASE_CORRECT


// Enabling micros() will cause the processor to interrupt more often (every 2048th clock cycle if
// F_CPU < 4.8 MHz, every 16384th clock cycle if F_CPU >= 4.8 MHz. This will add some overhead when F_CPU is
// less than 4.8 MHz. It's disabled by default because it occupies precious flash space and loads the CPU with
// additional interrupts and calculations. Also note that micros() aren't very precise for frequencies that 64
// doesn't divide evenly by.
// NOTE THAT THIS MACRO CAN BE OVERRIDDEN IN ARDUINO IDE TOOLS MENU
//#define ENABLE_MICROS

#endif
