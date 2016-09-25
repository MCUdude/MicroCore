/*** MicroCore - core_settings.h ***
Arduino core designed for ATtiny13
File created and maintained by MCUdude
https://github.com/MCUdude/MicroCore

This file lets you turn on and off core functions.
Since the ATtiny13 is really short on space, you
might want to disable some functions you don't
need, in order to free up some space.
*/

#ifndef core_settings_h
#define core_settings_h


// Makes the core "idiot proof" (see wiring_digital.c and wiring_pwm.h for examples where SAFEMODE is used).
// Enabling SAFEMODE takes up more flash space, but "makes sure" you doesn't screw up, like by reading the 
// state of a pin while it's outputting a PWM signal. If you know what you're doing, like explicitly writing
// a pin as an output before using the analogWrite() function, you can save a lot of space by disabling this.
#define SAFEMODE 

// If you're not using millis(), you can save about 100b by commenting this out.
// Note that the millis() interrupt is based on the watch dog timer, and will interrupt
// every 16th ms (which is very little. This means the millis() function will not be 
// accurate down to 1 ms, but will increase with steps of 16.
#define ENABLE_MILLIS

// WARNING!! Enabling micros() will cause the processor to interrupt every 256 clock cycle, and will mess
// up timing functions such as delay() and _delay_ms(). E.g delay(1) will actually last 1.3 ms instead.
// Do you really want this? The choice is always yours ;)
#define ENABLE_MICROS

// If you're not using the analog pins or want to set it up yourself, you can disable it here
#define SETUP_ADC

// If you're not using the PWM or want to set it up yourself, you can disable it here
#define SETUP_PWM

// Here's the PWM settings for Timer0
// These settings will also be disabled if SETUP_PWM is commented out
#define PRESCALER_8			  // PWM frequency = (F_CPU/256) / 8   <-- DEFAULT
//#define PRESCALER_NONE	// PWM frequency = (F_CPU/256) / 1
//#define PRESCALER_64		// PWM frequency = (F_CPU/256) / 64
//#define PRESCALER_256		// PWM frequency = (F_CPU/256) / 256
//#define PRESCALER_1024	// PWM frequency = (F_CPU/256) / 1024

// These are the waveform generation settings for timer0
#define PWM_FAST				//  <-- DEFAULT
//#define PWM_NORMAL
//#define PWM_PHASE_CORRECT
//#define PWM_CTC


#endif