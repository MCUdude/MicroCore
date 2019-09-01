/*** MicroCore - core_settings.h ***
Arduino core designed for ATtiny13
File created and maintained by MCUdude
https://github.com/MCUdude/MicroCore

This file lets you turn core functions on and off.
Since the ATtiny13 is really short on space, you
might want to disable some functions you don't
need, in order to free up some space.
*/

#ifndef core_settings_h
#define core_settings_h


// If you're not using millis(), you can save about 100b by commenting this out.
// Note that the millis() interrupt is based on the watch dog timer, and will interrupt
// every 16th ms (which is very little. This means the millis() function will not be 
// accurate down to 1 ms, but will increase with steps of 16.
// NOTE THAT THIS MACRO CAN BE OVERRIDDEN IN ARDUINO IDE
//#define ENABLE_MILLIS

// Enabling micros() will cause the processor to interrupt more often (every 2048th clock cycle if 
// F_CPU < 4.8 MHz, every 16384th clock cycle if F_CPU >= 4.8 MHz. This will add some overhead when F_CPU is
// less than 4.8 MHz. It's disabled by default because it occupies precious flash space and loads the CPU with
// additional interrupts and calculations. Also note that micros() aren't very precise for frequencies that 64
// doesn't divide evenly by.
// NOTE THAT THIS MACRO CAN BE OVERRIDDEN IN ARDUINO IDE
//#define ENABLE_MICROS

// Makes the core "idiot proof" (see wiring_digital.c and wiring_pwm.h for examples where SAFEMODE is used).
// Enabling SAFEMODE takes up more flash space, but "makes sure" you don't screw up, like by reading the 
// state of a pin while it's outputting a PWM signal. If you know what you're doing, like explicitly writing
// a pin as an output before using the analogWrite() function, you can save a lot of space by disabling this.
#define SAFEMODE 

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

// If you're not using the PWM or want to set it up yourself, you can disable it here
#define SETUP_PWM

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


#endif