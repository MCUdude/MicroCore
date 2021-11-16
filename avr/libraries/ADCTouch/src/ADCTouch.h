/*
  ADCTouch.h - Library for Capacittive touch sensors using a single ADC pin
  Created by MCUdude
  Based on the work done by martin2250 and nerdralph.
  Released into the public domain.
*/

#ifndef ADCTOUCH_H
#define ADCTOUCH_H

#include "Arduino.h"

// Maps analog pins to digital pins
const uint8_t PROGMEM analog_pin_to_digital_pin[] =
{
	5, // A0
	2, // A1
	4, // A2
	3, // A3
	NOT_A_PIN,
	NOT_A_PIN
};

class ADCTouch
{
	public:
	  ADCTouch();
	  static int16_t read(const analog_pin_t adc_channel);
	  static const uint16_t samples;

	private:
	  static void badArg(const char*) __attribute__((error("")));
};

extern ADCTouch Touch;

#endif
