# ADCTouch
ADCTouch is a light-weight, bare-bone capacitive touch library specifically written and optimized for ATtiny13. It allows users to create capacitive touch sensors without any external hardware. Each capacitive touch sensor requires a separate ADC pin.
See the [examples](https://github.com/MCUdude/MicroCore/tree/master/avr/libraries/ADCTouch/examples) on how this library can be utilized.

Developed by [MCUdude](https://github.com/MCUdude/). Based on the work done by [nerdralph](https://github.com/nerdralph/nerdralph/blob/master/avr/adctouch.c) and [martin2250](https://github.com/martin2250/ADCTouch).


## How it works
From [martin2250's library](https://github.com/martin2250/ADCTouch):

To Acquire a reading, the library does the following:
* Charge the test pin to VCC through the internal pullup resistor (not directly to prevent short circuits)
* Discharge the internal ~14pF capacitor
* Set the pin to tristate
* Connect the ~14pF capacitor with the pin so that charge distributes evenly
* Measure the voltage of the internal cap using the ADC.
  If the pin has a low capacitance, the stored charge will be small as will the resulting voltage, if the external capacitance is equal to 14pF, the volatage should be ~2.5V. Even higher capacitances will result in voltages > 2.5V. The ATtiny13 already has stray capacitances that will produce an output of ~390 and just a single external wire can boost that up to 500, so you really need offset compensation.
  The accuracy is really good, most often even the LSB/smalles digit can still yield usable data and just vary by a single unit between readings (at only 64 samples, more will mean even less variation). The sensitivity is phenomenal, with a large enough surface, it can sense a person in more than 2 feet distance.


## samples
Static constant to set how many samples each reading will do before it returns the result.
Note that the assigned value must be a power of two.

### Declaration
``` c++
static const uint16_t samples;
```

### Usage
``` c++
const uint16_t ADCTouch::samples = 32; // Sample each touch pin 32 times
```


## read()
Function that reads an ADC pin and returns an average reading based on the number of samples.
Parameter has to be a `analog_pin_t` constant, `A0` to `A5`.

### Declaration
``` c++
static int16_t read(const analog_pin_t adc_channel);
```

### Usage
``` c++
int16_t reading = Touch.read(A2); // Read touch on analog pin A2
```
