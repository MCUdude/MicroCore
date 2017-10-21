/*** MicroCore - pins_arduino.h ***
An Arduino core designed for ATtiny13
Based on the work done by "smeezekitty" 
Modified and maintained by MCUdude
https://github.com/MCUdude/MicroCore

This file contains ATtiny13/A
specific hardware definitions.
               __ __
(D5/A0) PB5  1|     |8  VCC
(D3/A3) PB3  2|     |7  PB2 (D2/A1)
(D4/A2) PB4  3|     |6  PB1 (D1/PWM)
        GND  4|_____|5  PB0 (D0/PWM)          
*/

// Boards.txt defines ATtiny13A. This ensures compatibility
#if !defined(__AVR_ATtiny13__)
#define __AVR_ATtiny13__
#endif

// IO, ADC, INT and PWM
#define NUM_DIGITAL_PINS           6
#define NUM_ANALOG_INPUTS          4
#define EXTERNAL_NUM_INTERRUPTS    1
#define digitalPinToInterrupt(p)   (((p) == 1) ? 0 : NOT_AN_INTERRUPT)
#define analogInputToDigitalPin(p) (((p) == 0) ? 5 : (((p) == 1) ? 2 : (((p) == 2) ? 4 :(((p) == 3) ? 3 : -1))))
#define analogPinToChannel(p)      (p)
#define digitalPinHasPWM(p)        (((p) == 0) || ((p) == 1))

// Port and pin mapping
#if defined(SAFEMODE)
  #define digitalPinToPort(p)      (((p) >= 0 && (p) <= 5) ? 2 : 0)
  #define digitalPinToBitMask(p)   (((p) >= 0 && (p) <= 5) ? _BV(p) : -1)
  #define digitalPinToTimer(p)     (((p) == 0) ? TIMER0A : (((p) == 1) ? TIMER0B : (NOT_ON_TIMER)))
  #define analogInPinToBit(p)      (p)  
  #define portOutputRegister(p)    (((p) == 2) ? (&PORTB) : ((uint8_t *)NULL))
  #define portInputRegister(p)     (((p) == 2) ? (&PINB)  : ((uint8_t *)NULL))
  #define portModeRegister(p)      (((p) == 2) ? (&DDRB)  : ((uint8_t *)NULL))
#else
  #define digitalPinToPort(p)      (2)
  #define digitalPinToBitMask(p)   (_BV(p))
  #define digitalPinToTimer(p)     (p + 1)
  #define analogInPinToBit(p)      (p)
  #define portOutputRegister(p)    (&PORTB)
  #define portInputRegister(p)     (&PINB)
  #define portModeRegister(p)      (&DDRB)
#endif

// Analog reference macros
#define EXTERNAL 0
#define INTERNAL 1
#define INTERNAL1V1 1

// PCINT implementation
#if defined(SAFEMODE)
  #define digitalPinToPCICR(p)     (((p) >= 0 && (p) <= 5) ? (&GIMSK) : ((uint8_t *)NULL))
  #define digitalPinToPCMSK(p)     (((p) >= 0 && (p) <= 5) ? (&PCMSK) : ((uint8_t *)NULL))
#else
  #define digitalPinToPCICR(p)     (&GIMSK)
  #define digitalPinToPCMSK(p)     (&PCMSK)
#endif
#define digitalPinToPCICRbit(p)    (5)
#define digitalPinToPCMSKbit(p)    (p)


#define LED_BUILTIN 0
static const uint8_t MOSI = 0;
static const uint8_t MISO = 1;
static const uint8_t SCK  = 2;

// Ax constants cannot be used for digitalRead/digitalWrite/analogWrite functions, only analogRead()
static const uint8_t A0 = 0;
static const uint8_t A1 = 1;
static const uint8_t A2 = 2;
static const uint8_t A3 = 3;

