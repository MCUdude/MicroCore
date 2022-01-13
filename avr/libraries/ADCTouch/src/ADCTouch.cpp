/*
  ADCTouch.cpp - Library for Capacittive touch sensors using only one ADC PIN
  Created by martin2250, April 23, 2014.
  Released into the public domain.
*/

#include "ADCTouch.h"

// Predefined object
ADCTouch Touch;

ADCTouch::ADCTouch()
{

}

int16_t ADCTouch::read(const analog_pin_t adc_channel)
{
  // Compile time check for number of samples
  if(samples == 0 || samples > 512 || (samples & (samples - 1)))
    badArg("Number of samples must be a power of two, greater than zero and max 512!");

  uint16_t samples_left = samples;
  int16_t level = 0;
  uint8_t digitalPin = pgm_read_byte(analog_pin_to_digital_pin + adc_channel);
  do
  {
    ADMUX = adc_channel;                // Select ADC channel
    DDRB |= (1 << digitalPin);          // Discharge touchpad

    ADCSRA |= (1 << ADEN);              // Enable ADC & discharge S/H cap
    _delay_us(1);                       // Sample and hold RC time
    ADCSRA = 0;                         // ADC off
    PORTB |= (1 << digitalPin);         // Charge touchpad

    DDRB  &= ~(1 << digitalPin);        // Input mode
    PORTB &= ~(1 << digitalPin);        // Pullup off

    // Enable ADC with /16 prescaler, equalize S/H cap charge
    ADCSRA = (1 << ADPS2) | (0 << ADPS1) | (0 << ADPS0) | (1 << ADEN);
    _delay_us(1);                       // Sample and hold RC time

    ADCSRA |= (1 << ADSC);              // Start ADC conversion
    while (bit_is_set(ADCSRA, ADSC));   // Wait for conversion to complete
    level += ADCW;
    ADCSRA = 0;                         // ADC off
  }
  while(--samples_left);
    return level / samples;
}
