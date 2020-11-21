/*** MicroCore - wiring_analog.c ***
An Arduino core designed for ATtiny13
Based on the work done by "smeezekitty"
Modified and maintained by MCUdude
https://github.com/MCUdude/MicroCore

This file contains analog related
functions such as analogRead()
and analogReference().
*/

#include "wiring_private.h"
#include "core_settings.h"


void analogReference(uint8_t mode)
{
  if (mode == DEFAULT)
    ADMUX &= ~(1 << REFS0); // Vcc as reference
  else
    ADMUX |= (1 << REFS0); // Internal 1.1V as reference
}


int16_t analogRead(analog_pin_t pin)
{
  check_valid_analog_pin(pin);

  // MUX1 & MUX0 are 2 lowest bits in ADMUX
  ADMUX = (ADMUX & 0xFC) | pin;

  #if defined(ADC_PRESCALER_2)         // ADC prescaler 2
    ADCSRA = _BV(ADEN) | _BV(ADSC);
  #elif defined(ADC_PRESCALER_4)       // ADC prescaler 4
    ADCSRA = _BV(ADEN) | _BV(ADSC) | _BV(ADPS1);
  #elif defined(ADC_PRESCALER_8)       // ADC prescaler 8
    ADCSRA = _BV(ADEN) | _BV(ADSC) | _BV(ADPS1) | _BV(ADPS0);
  #elif defined(ADC_PRESCALER_16)      // ADC prescaler 16
    ADCSRA = _BV(ADEN) | _BV(ADSC) | _BV(ADPS2);
  #elif defined(ADC_PRESCALER_32)      // ADC prescaler 32
    ADCSRA = _BV(ADEN) | _BV(ADSC) | _BV(ADPS2) | _BV(ADPS0);
  #elif defined(ADC_PRESCALER_64)      // ADC prescaler 64
    ADCSRA = _BV(ADEN) | _BV(ADSC) | _BV(ADPS2) | _BV(ADPS1);
  #elif defined(ADC_PRESCALER_128)     // ADC prescaler 128
    ADCSRA = _BV(ADEN) | _BV(ADSC) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
  #else // (ADC_PRESCALER_AUTO)        // "Automatic" ADC prescaler option
    #if F_CPU <= 200000L               // Less or equal to 200 kHz
      ADCSRA = _BV(ADEN) | _BV(ADSC);  // Prescaler of 2 -> F_CPU / 2
    #elif F_CPU <= 1200000L            // Between 200 kHz and 1.2 MHz
      ADCSRA = _BV(ADEN) | _BV(ADSC) | _BV(ADPS1); // Prescaler to 4 -> F_CPU / 4
    #elif F_CPU <= 6400000L            // Between 1.2 MHz and 6.4 MHz
      ADCSRA = _BV(ADEN) | _BV(ADSC) | _BV(ADPS2); // Prescaler to 16 -> F_CPU / 16
    #else                              // Greater than 6.4 MHz
      ADCSRA = _BV(ADEN) | _BV(ADSC) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0); // Prescaler to 128 -> F_CPU / 128
    #endif
  #endif

  while(ADCSRA & _BV(ADSC)); // Wait for conversion
  int16_t result = ADCW;
  ADCSRA = 0; // turn off ADC
  return result;
}
