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

uint8_t analog_reference = DEFAULT;

void analogReference(uint8_t mode)
{
  // We can't just set the analog reference because the
  // default setting will internally connect AREF and AVCC,
  // which may cause a short if something is connected to the
  // AREF pin

  analog_reference = mode;
}


int16_t analogRead(uint8_t pin)
{
  #if defined(ADC_PRESCALER_2)         // ADC prescaler 2
    ADCSRA = _BV(ADEN);
  #elif defined(ADC_PRESCALER_4)       // ADC prescaler 4
    ADCSRA = _BV(ADEN) | _BV(ADPS1);
  #elif defined(ADC_PRESCALER_8)       // ADC prescaler 8
    ADCSRA = _BV(ADEN) | _BV(ADPS1) | _BV(ADPS0);
  #elif defined(ADC_PRESCALER_16)      // ADC prescaler 16
    ADCSRA = _BV(ADEN) | _BV(ADPS2);
  #elif defined(ADC_PRESCALER_32)      // ADC prescaler 32
    ADCSRA = _BV(ADEN) | _BV(ADPS2) | _BV(ADPS0);
  #elif defined(ADC_PRESCALER_64)      // ADC prescaler 64
    ADCSRA = _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1);
  #elif defined(ADC_PRESCALER_128)     // ADC prescaler 128
    ADCSRA = _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
  #else // (ADC_PRESCALER_AUTO)        // "Automatic" ADC prescaler option
    #if F_CPU <= 200000                // Less or equal to 200 kHz
      ADCSRA = _BV(ADEN);              // Prescaler of 2 --> F_CPU / 2
    #elif F_CPU <= 1200000             // Between 200 kHz and 1.2 MHz
      ADCSRA = _BV(ADEN) | _BV(ADPS1); // Prescaler to 4 --> F_CPU / 4
    #elif F_CPU <= 6400000             // Between 1.2 MHz and 6.4 MHz
      ADCSRA = _BV(ADEN) | _BV(ADPS2); // Prescaler to 16 --> F_CPU / 16
    #else                              // Greater than 6.4 MHz
      ADCSRA = _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0); // Prescaler to 128 --> F_CPU / 128
    #endif
  #endif

  uint8_t l, h;
  ADMUX = (analog_reference << REFS0) | (pin & 0x03); // Select channel and reference
  ADCSRA |= _BV(ADSC); // Start conversion
  while(ADCSRA & _BV(ADSC)); // Wait for conversion
  l = ADCL;  // Read and return 10 bit result
  h = ADCH;
  return (h << 8) | l; 
}


    
