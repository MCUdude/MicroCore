/*** MicroCore - core_settings.h ***
Arduino core designed for ATtiny13
File created and maintained by MCUdude

This file lets you turn on and off core functions.
Since the ATtiny13 is really short on space, you
might want to disable some functions you don't
need anyway to save space.
*/

#ifndef core_settings_h
#define core_settings_h


// Makes the core "idiot proof" (see wiring_digital.c), but takes up more space and resources if enabled
#define SAFEMODE 

// If you're not using millis() and micros(), you can save more than 100b by commenting this out
#define ENABLE_MILLIS

// If you're not using the analog pins or want to set it up yourself, you can disable it here
#define SETUP_ADC

// If you're not using the PWM or want to set it up yourself, you can disable it here
#define SETUP_PWM


#endif