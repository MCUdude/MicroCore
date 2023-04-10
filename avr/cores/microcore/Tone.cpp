/*** MicroCore - Tone.cpp ***
An Arduino core designed for ATtiny13
Based on the work done by "smeezekitty"
Modified and maintained by MCUdude
https://github.com/MCUdude/MicroCore
Optimized for size by Nerd Ralph

Simplified Tone "Library"
The Tone.cpp file in the official Arduino core is a complete and utter mess,
once again the developers over the years have just used a load of
arcane #ifdef's to try and work out what they should do for a specific chip.

Which pretty much makes it totally completely unreasonably unreadable.

So we simplify by replacing the existing Tone with our own, we are not
composing a grapnd symphony here, let's face it, blurting out Mario Bros
is about the most people use tone() for!

This modified version of Tone.cpp is released under the MIT license (MIT).

@author James Sleeman <james@gogo.co.nz> (https://github.com/sleemanj)
@license The MIT License (MIT)
*/

#include "Arduino.h"

static __uint24 CurrentToneDuration = 0;
static uint8_t  CurrentTonePin      = 0;
static uint8_t  CurrentToneMidpoint = 0;

#ifndef TONE_MIN_FREQ
  #define TONE_MIN_FREQ 130 // 130.81 = C3
#endif

#ifndef TONE_MAX_FREQ
  #define TONE_MAX_FREQ 2093 // 2093.0 = C6
#endif

/**
 * @brief Generates a square wave of the specified frequency (and 50% duty
 *        cycle) on a pin. A duration can be specified, otherwise the wave
 *        continues until a call to noTone().
 *
 * @param pin The Arduino pin on which to generate the tone
 * @param frequency  Tone frequency in [Hz]
 * @param length The duration of the tone in milliseconds (optional)
 */
void tone(const uint8_t pin, const uint16_t frequency, const uint32_t length)
{
  if(frequency == 0)
    return noTone(pin);

  uint16_t  prescaleDivider = 1; // The divider increases by powers of 2
  uint8_t   prescaleBitMask = 1; // The bitmask only increments unitarily
  uint32_t  ticksPerMs      = F_CPU / 1000;

  // The numbers in each if statement are the lowest frequency that can be achieved
  // with the prescale inside the if statement
  //
  // eg: the frequency can be viewed as a number of transitions equal to the
  //     frequency * ([LOW]-transition-[HIGH])
  //     (we ignore the extra transition between cycles, happens anyway, err, I think, maybe, not sure)
  //     the longer the time between transitions the lower the frequency
  //     the maximum length we can get is 255 ticks
  //     the minimum length we can get is 1 tick
  //
  //     therefore minimum is CPU_FREQ / Prescale / 255
  //               maximum is CPU_FREQ / Prescale / 1

  // We will add/remove 1 hz to account for rounding here

  #define TONE_MIN_FREQ_FOR_PRESCALE(PS) (((F_CPU / PS) / 255) + 1)
  #define TONE_MAX_FREQ_FOR_PRESCALE(PS) (((F_CPU / PS) / 1)   - 1)

  #define TONE_RANGE_ACTIVE_FOR_PRESCALE(PS) ( TONE_MAX_FREQ >= TONE_MIN_FREQ_FOR_PRESCALE(PS) && TONE_MIN_FREQ <= TONE_MAX_FREQ_FOR_PRESCALE(PS) )

  if(TONE_RANGE_ACTIVE_FOR_PRESCALE(1) && frequency >= TONE_MIN_FREQ_FOR_PRESCALE(1))     // (F_CPU / 1) / 255
  {
    prescaleDivider = 1;
    ticksPerMs      = F_CPU / 1000;
    prescaleBitMask = 1;
  }
  else if(TONE_RANGE_ACTIVE_FOR_PRESCALE(8) && frequency >= TONE_MIN_FREQ_FOR_PRESCALE(8))  // (F_CPU / 8) / 255
  {
    prescaleDivider = 8;
    ticksPerMs      = (F_CPU/8) / 1000;
    prescaleBitMask = 2;
  }
  else if(TONE_RANGE_ACTIVE_FOR_PRESCALE(64) && frequency >= TONE_MIN_FREQ_FOR_PRESCALE(64))  // (F_CPU / 64) / 255
  {
    prescaleDivider = 64;
    ticksPerMs      = (F_CPU/64) / 1000;
    prescaleBitMask = 3;
  }
  else if(TONE_RANGE_ACTIVE_FOR_PRESCALE(256) && frequency >= TONE_MIN_FREQ_FOR_PRESCALE(256))  // (F_CPU / 256) / 255
  {
    prescaleDivider = 256;
    ticksPerMs      = (F_CPU/256) / 1000;
    prescaleBitMask = 4;
  }
  else if(TONE_RANGE_ACTIVE_FOR_PRESCALE(1024)  && frequency >= TONE_MIN_FREQ_FOR_PRESCALE(1024))  // (F_CPU / 1024) / 255
  {
    prescaleDivider = 1024;
    ticksPerMs      = (F_CPU/1024) / 1000;
    prescaleBitMask = 5;
  }
  else
    return;

  toneRaw(pin, (((F_CPU / prescaleDivider) / frequency) / 2), length ? length * ticksPerMs : ~(0UL), prescaleBitMask);
}


void toneRaw(uint8_t pin, uint8_t midPoint, uint32_t lengthTicks, uint8_t prescaleBitMask)
{

  // Because the t13 is so limited in flash space, this really is little more than
  // a because-I-can excercise in futility.
  //
  // Because we can't do division (way too heavy) an inline function is used to
  // do pre-calculation of the midPoint (of the timer, where we toggle the pin)
  // the number of ticks, and the prescale bitmask.  Naturally this is only
  // going to work when tone is fed a constant frequency and length!

  // Because we only have one timer, we can't just use millis() to
  // do tone duration, since it won't be accurate.
  //
  // So instead we calculate how many ticks the tone should go for and subtract
  // ticks as we hit the mid-point.

  CurrentToneDuration = lengthTicks;

  CurrentTonePin = _BV(pin);
  pinMode(pin, OUTPUT);

  // Shut down interrupts while we fiddle about with the timer.
  cli();

  TCCR0B &= ~0b00000111; // Turn off the timer before changing anytning
  TCNT0   = 0;           // Timer counter back to zero

  // Set the comparison, we will flip the bit every time this is hit
  // (actually, this will set TOP of the timer and we flip on the overflow)
  OCR0A = midPoint;
  CurrentToneMidpoint = midPoint;

  // Enable the overflow interrupt
  TIMSK0 |= _BV(OCIE0A);

  // Start playing and record time
  digitalWrite(pin, HIGH);

  // Start the timer
  TCCR0A = 0b00000011;
  TCCR0B = 0b00001000 | prescaleBitMask;

  sei(); // We **have** to enable interrupts here even if they were disabled coming in,
         // otherwise it's not going to do anything.  Hence not saving/restoring SREG.
}

/**
 * @brief Stops the generation of a square wave triggered by tone().
 *
 * @param pin The Arduino pin on which to stop generating the tone
 */
void noTone(uint8_t pin)
{
  // Disable the interrupt
  // Note we can leave the rest of the timer setup as is, turnOnPWM() will
  // fix it for itself next time you analogWrite() if you need to.
  TIMSK0 &= ~_BV(OCIE0A);

  // Pin goes back to input and low state
  DDRB &= ~pin;
  PORTB &= ~pin;
}


/**
 * @brief Resets Timer0 state back to its default MicroCore setting
 */
void stopTone()
{
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
}


ISR(TIM0_COMPA_vect)
{
  auto toneDuration = CurrentToneDuration;
  auto tonePin      = CurrentTonePin;
  auto toneMidpoint = CurrentToneMidpoint;
  // Toggle the pin, most AVR can toggle an output pin by writing a 1 to the input
  // register bit for that pin.
  PINB = CurrentTonePin;

  // If we have played this tone for the requested duration, stop playing it.
  if (toneDuration < toneMidpoint)
    noTone(tonePin);
  CurrentToneDuration = toneDuration - toneMidpoint;

  TCNT0 = 0; // Restart timer
}
