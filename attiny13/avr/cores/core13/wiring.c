/*
*** Core13 ***
Arduino core designed for Attiny13 and similar devices.
NO WARRANTEE OR GUARANTEES!
Written by John "smeezekitty" 
You are free to use, redistribute and modify at will EXCEPT IF MARKED OTHERWISE IN A PARTICULAR SOURCE FILE!
Version 0.22
// ATMEL ATTINY13 - ARDUINO
//
//                  +-\/-+
// AinX (D X) PB5  1|    |8  Vcc
// AinX (D X) PB3  2|    |7  PB2 (D X)  AinX
// AinX (D X) PB4  3|    |6  PB1 (D X) PWM
//            GND  4|    |5  PB0 (D X) PWM
//                  +----+
*/

#include "wiring_private.h"
#include <avr/interrupt.h>
volatile unsigned long ovrf=0;
ISR(TIM0_OVF_vect){
	ovrf++; //Increment counter every 256 clock cycles
}
unsigned long millis(){
	unsigned long x;
	asm("cli"); 
	/*Scale number of timer overflows to milliseconds*/
	#if F_CPU < 150000 && F_CPU > 80000
	x = ovrf * 2;
    #elif F_CPU == 600000
	x = ovrf / 2;
	#elif F_CPU == 1000000
	x = ovrf / 4;
	#elif F_CPU == 1200000
	x = ovrf / 5;
	#elif F_CPU == 4000000
	x = ovrf / 16;
	#elif F_CPU == 4800000
	x = ovrf / 19;
	#elif F_CPU == 8000000
	x = ovrf / 31;
	#elif F_CPU == 9600000
	x = ovrf / 37;
    #elif F_CPU == 10000000
	x = ovrf / 39;
	#elif F_CPU == 12000000
	x = ovrf / 47;
	#elif F_CPU == 16000000
	x = ovrf / 63;
	#else
	#error This CPU frequency is not defined
	#endif
	asm("sei");
	return x;
}
/*The following improved micros() code was contributed by a sourceforge user "BBC25185" Thanks!*/
unsigned long micros(){
	unsigned long x;
	asm("cli");
	#if F_CPU < 150000 && F_CPU > 80000
	x = ovrf * 2000;
	#elif F_CPU == 600000
	x = ovrf * 427;
	#elif F_CPU == 1000000
	x = ovrf * 256;
	#elif F_CPU == 1200000
	x = ovrf * 213;
	#elif F_CPU == 4000000
	x = ovrf * 64;
	#elif F_CPU == 4800000
	x = ovrf * 53;
	#elif F_CPU == 8000000
	x = ovrf * 32;
	#elif F_CPU == 9600000
	x = ovrf * 27;
	#elif F_CPU == 10000000
	x = ovrf * 26;
	#elif F_CPU == 12000000
	x = ovrf * 21;
	#elif F_CPU == 16000000
	x = ovrf * 16;
	#else 
	#error This CPU frequency is not defined
	#endif
	asm("sei");
	return x; 
}
void delay(unsigned ms){
	while(ms--){
		_delay_ms(1); //Using the libc routine over and over is non-optimal but it works and is close enough
	}//Note, I may have to reimplement this because the avr-libc delay is too slow *todo*
}
//For bigger delays. Based on code by "kosine" on the Arduino forum
void uS_new(unsigned us)
{
uint8_t us_loops;  // define the number of outer loops based on CPU speed (defined in boards.txt)
  #if (F_CPU == 16000000L) 
    us_loops=16;
  #elif (F_CPU == 9600000L) || (F_CPU == 10000000)
    us=us + (us >> 3); // this should be *1.2 but *1.125 adjusts for overheads
    us_loops=8;
  #elif (F_CPU == 8000000L) 
    us_loops=8;
  #elif (F_CPU == 4800000L) 
    us = us + (us >> 3);
    us_loops=4;
  #elif (F_CPU == 4000000L)
     us_loops=4;
  #endif

  us = us >> 2;
  uint8_t us_low = us & 255;
  uint8_t us_high = us >> 8;
    
  // loop is (4 clock cycles) + (4x us) + (4x us_loops)
  // each clock cycle is 62.5ns @ 16MHz
  // each clock cycle is 833.3ns @ 1.2MHz
  asm volatile(
   "CLI\n" // turn off interrupts : 1 clock
   "MOV r28,%0\n" // Store low byte into register Y : 1 clock
   "MOV r29,%1\n" // Store high byte into register Y : 1 clock
   "MOV r30,%2\n" // Set number of loops into register Z : 1 clock
  
     // note branch labels MUST be numerical (ie. local) with BRNE 1b (ie. backwards)
     "1:\n" // : 4 clock cycles for each outer loop
     "MOV r26,r28\n" // Copy low byte into register X : 1 clock
     "MOV r27,r29\n" // Copy high byte into register X : 1 clock
    
       "2:\n" // : 4 clock cycles for each inner loop
       "SBIW r26,1\n" // subtract one from word : 2 clocks
       "BRNE 2b\n" // Branch back unless zero flag was set : 1 clock to test or 2 clocks when branching
       "NOP\n" // add an extra clock cycle if not branching
      
     "SUBI r30,1\n" // subtract one from loop counter : 1 clocks
     "BRNE 1b\n" // Branch back unless zero flag was set : 1 clock to test or 2 clocks when branching 

   "SEI\n" // turn on interrupts : 1 clock (adds extra clock cycle when not branching)
   :: "r" (us_low), "r" (us_high), "r" (us_loops) // tidy up registers
 );
}
void delayMicroseconds(int us){
	if(us == 0){return;}
	#if F_CPU == 16000000 || F_CPU == 12000000
	if(us > 99){uS_new(us); return;}
	us--;
	us <<= 2;
	//us -= 2; 
	#elif F_CPU == 8000000 || F_CPU == 9600000 || F_CPU == 10000000
	if(us > 199){uS_new(us); return;}
	us-=3;
	us <<= 1;
	//us--; //underflow possible?
	#elif F_CPU == 4000000 || F_CPU == 4800000
	if(us > 299){uS_new(us); return;}
	us-=6;
	//For 4MHz, 4 cycles take a uS. This is good for minimal overhead
	#elif F_CPU == 1000000 || F_CPU == 1200000//For slow clocks, us delay is marginal.
	us-=16;
	us >>= 2; 
	//us--; //Underflow?
	#elif F_CPU == 600000
	us-=32;
	us >>= 3;
	#elif F_CPU < 150000 && F_CPU > 80000
	us-=125;
	us >>= 5;
	#else 
	#error Invalid F_CPU value
	#endif
	if(us < 0){return;} //Ran out of time
	asm __volatile__("1: sbiw %0,1\n\t"
			 "brne 1b" : "=w" (us) : "0" (us));
}
void init(){
	//Setup timer interrupt and PWM pins
	TCCR0B |= _BV(CS00);
	TCCR0A |= _BV(WGM00)|_BV(WGM01);
	TIMSK0 |= 2;
	TCNT0=0; 
	sei();
	ADMUX=0;
	//Set up ADC clock depending on F_CPU
	#if F_CPU <= 200000
	ADCSRA |= _BV(ADEN);
	#elif F_CPU <= 1200000 && F_CPU > 200000
	ADCSRA |= _BV(ADEN) | _BV(ADPS1);
	#elif F_CPU > 1200000 && F_CPU < 6400001
	ADCSRA |= _BV(ADEN) | _BV(ADPS2);
	#else
	ADCSRA |= _BV(ADEN) | _BV(ADPS1) | _BV(ADPS0) | _BV(ADPS2);
	#endif
}
	