/*** MicroCore - wiring_pwm.c ***
An Arduino core designed for ATtiny13
File created and maintained by MCUdude
https://github.com/MCUdude/MicroCore

This file contains the PWM function
analogWrite().
*/

#include "wiring_private.h"
#include "core_settings.h"


void turnOffPWM(uint8_t pin)
{
  check_valid_pwm_pin(pin);

  if(pin == 0)
    TCCR0A &= ~_BV(COM0A1);
  else
    TCCR0A &= ~_BV(COM0B1);
}


void analogWrite(uint8_t pin, uint8_t val)
{
  check_valid_pwm_pin(pin);

  // Set Timer0 prescaler
  #if !defined(ENABLE_MICROS)
    #if defined(PWM_PRESCALER_NONE)     // PWM frequency = (F_CPU/256) / 1
      TCCR0B = _BV(CS00);
    #elif defined(PWM_PRESCALER_8)      // PWM frequency = (F_CPU/256) / 8
      TCCR0B = _BV(CS01);
    #elif defined(PWM_PRESCALER_64)     // PWM frequency = (F_CPU/256) / 64
      TCCR0B = _BV(CS00) | _BV(CS01);
    #elif defined(PWM_PRESCALER_256)    // PWM frequency = (F_CPU/256) / 256
      TCCR0B = _BV(CS02);
    #elif defined(PWM_PRESCALER_1024)   // PWM frequency = (F_CPU/256) / 1024
      TCCR0B = _BV(CS00) | _BV(CS02);
    #else // (PWM_PRESCALER_AUTO)       // Automatic prescaler calculation
      #if F_CPU >= 4800000L
        TCCR0B = _BV(CS00) | _BV(CS01); // PWM frequency = (F_CPU/256) / 64
      #else
        TCCR0B = _BV(CS01);             // PWM frequency = (F_CPU/256) / 8
      #endif
    #endif
  #endif

  pinMode(pin, OUTPUT);

  // Handle off condition
  if(val == 0)
  {
    turnOffPWM(pin);        // Turn off PWM
    digitalWrite(pin, LOW); // Set pin low
  }

  // Handle on condition
  else if(val == 255)
  {
    turnOffPWM(pin);         // Turn off PWM
    digitalWrite(pin, HIGH); // Set pin high
  }

  // Otherwise setup the appropriate timer compare
  else
  {
    if(pin == 0)
    {
      // Set waveform generation mode and output number
      #if defined(PWM_PHASE_CORRECT)
        TCCR0A |= _BV(WGM00) | _BV(COM0A1);
      #else // (PWM_FAST)
        TCCR0A |= _BV(WGM00) | _BV(WGM01) | _BV(COM0A1);
      #endif
      OCR0A = val;
    }
    else
    {
      // Set waveform generation mode and output number
      #if defined(PWM_PHASE_CORRECT)
        TCCR0A |= _BV(WGM00) | _BV(COM0B1);
      #else // (PWM_FAST)
        TCCR0A |= _BV(WGM00) | _BV(WGM01) | _BV(COM0B1);
      #endif
      OCR0B = val;
    }
  }
}
